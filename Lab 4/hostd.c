#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "hostd.h"

#define MEMORY 1024


int avail_mem[MEMORY] = {0};

int PRINTERS = 2;
int SCANNERS = 1;
int MODEMS = 1;
int CD_DRIVES = 2;

struct queue *realTime;
struct queue *priorityQueue1;
struct queue *priorityQueue2;
struct queue *priorityQueue3;


int findFreeMemoryIndex(int memSize,int realTime){
    int count =0;
    int isDone =0;
    int startingLoc = 0;
    if(realTime==1){
        for(int i=MEMORY;i>=MEMORY-64;i--){
            if(avail_mem[i]==0){
                count ++;
            }
            if(avail_mem[i]==1){
                count =0;
                startingLoc = i;
            }
            
            if(count==memSize){
                startingLoc = i;
                isDone =1;
                break;
            }
        }
        
    } else {
        for(int i=0;i<MEMORY-64;i++){
            if(avail_mem[i]==0){
                count ++;
            }
            if(avail_mem[i]==1){
                //                printf("\nreset memcounter at %d\n",i);
                count =0;
                startingLoc = i;
            }
            
            if(count==memSize){
                
                isDone =1;
                break;
            }
        }
        
    }
    if(isDone==1){
        return startingLoc;
    } else {
        return -1;
    }
}

void allocateMemory(int fromIndex, int size) {
    int indexToReturn = 0;
//    printf("aloccationg from index %d\n",fromIndex);
    for(int i=fromIndex;i<fromIndex+size;i++){
        // if(avail_mem[i]==0){
        //     indexToReturn = i;
        
        // }
        avail_mem[i] = 1;
    }
    
    
}

void freeMemory(int fromIndex, int size) {
    int indexToReturn = 0;
//    printf("free from index %d\n",fromIndex);
    for(int i=fromIndex;i<fromIndex+size;i++){
        // if(avail_mem[i]==0){
        //     indexToReturn = i;
        
        // }
        avail_mem[i] = 0;
    }
    
    
}

int checkEnoughMemory(int memSize, int realTime) {
    int count=0;
    int isFree=0;
    int startingLoc = 0;
    if(realTime==1){
        for(int i=MEMORY;i>=MEMORY-64;i--){
            if(avail_mem[i]==0){
                count ++;
            }
            if(count==memSize){
                isFree=1;
                break;
            }
        }
        
    } else {
        
    }
    return isFree;
}

void printFreeMemory(){
    int count =0;
    for(int i=0;i<=960;i++){
        if(avail_mem[i]==0){
            count ++;
        }
        
    }
//    printf("\nfree memory = %d\n",count);
}

void printProc(struct proc item){
    printf("arrivaltime: %d, priority %d,address: %d, processorTime %d, memory %d,printers %d\,scanners %d\,modems %d\,cds %d,pid %d\n",item.arrivalTime,item.priority,item.address,item.processorTime,item.memory,item.printers,item.scanners,item.modems,item.cds,item.pid);
}

void executeRealTimeProcess(struct proc realTimeItem,int runTime){
    int pid;
    int status;
    realTimeItem.processorTime = realTimeItem.processorTime - runTime;
    pid = fork();
    
    if(pid==0){
        signal(SIGINT, sigint);
        char *args[]={ "./process",NULL};
        execvp(args[0],args);
        
    } else {
        realTimeItem.pid =pid;
        sleep(runTime);
        kill(pid,SIGINT);
        waitpid(realTimeItem.pid, &status, WUNTRACED);
//        printf("done\n");
    }
}

void executeProcess(struct proc *realTimeItem,int runTime){
    int pid;
    int status;
    realTimeItem->processorTime = realTimeItem->processorTime - runTime;
    pid = fork();
    
    if(pid==0){
        signal(SIGTSTP, sigstp);
        signal(SIGCONT, sigCont);
        signal(SIGINT, sigint);
        
        char *args[]={ "./process",NULL};
        execvp(args[0],args);
        
    } else {
//        printf("child pid = %d\n",pid);
        realTimeItem->pid =pid;
        sleep(runTime);
        if(realTimeItem->processorTime!=0){
            kill(pid,SIGTSTP);
            waitpid(realTimeItem->pid, &status, WUNTRACED);
        } else {
            realTimeItem->pid = -1;
            kill(pid,SIGINT);
            waitpid(realTimeItem->pid, &status, WUNTRACED);
            freeMemory(realTimeItem->address,realTimeItem->memory);
            
        }
        
        
        
//        printf("done\n");
    }
}

void runExistingProcess(struct proc *realTimeItem) {
    int status;
    printProc(*realTimeItem);
    realTimeItem->processorTime -=1;
    kill(realTimeItem->pid,SIGCONT);
    
    sleep(1);
    if(realTimeItem->processorTime==0){
        kill(realTimeItem->pid,SIGINT);
        waitpid(realTimeItem->pid, &status, WUNTRACED);
        freeMemory(realTimeItem->address,realTimeItem->memory);
        
    } else {
        kill(realTimeItem->pid,SIGTSTP);
        waitpid(realTimeItem->pid, &status, WUNTRACED);
    }
}

int checkResources (struct proc *realTimeItem) {
  
    
    int printerAvailable = 0;
     int scannerAvailable = 0;
     int modemAvailable = 0;
     int cdAvailable = 0;
    if(realTimeItem->printers!=0){
        if(realTimeItem->printers<=PRINTERS){
            printerAvailable = 1;
        }
    } else {
        printerAvailable = 1;
    }
    if(realTimeItem->scanners!=0){
        if(realTimeItem->scanners<=SCANNERS){
            scannerAvailable = 1;
        }
           
       }else {
           scannerAvailable = 1;
       }
    if(realTimeItem->modems!=0){
        if(realTimeItem->modems<=MODEMS){
            modemAvailable = 1;
        }
        
    }else {
        modemAvailable = 1;
    }
    if(realTimeItem->cds!=0){
        if(realTimeItem->cds<=CD_DRIVES){
            cdAvailable = 1;
        }
        
    }else {
        cdAvailable = 1;
    }
    
    
    
    return printerAvailable & scannerAvailable &modemAvailable&cdAvailable;
    
    
    
}

void allocateResources(struct proc *realTimeItem){
//    printf("\n+++++++++++ allocating resources +++++++++\n");
//    printf("available BEFORE ALLOC printers %d , scanners %d, modems %d ,cd %d\n",PRINTERS,SCANNERS,MODEMS,CD_DRIVES);
    PRINTERS -= realTimeItem->printers;
    SCANNERS -= realTimeItem->scanners;
    MODEMS -= realTimeItem->modems;
    CD_DRIVES -= realTimeItem->cds;
//     printf("available AFTER ALLOC printers %d , scanners %d, modems %d ,cd %d\n",PRINTERS,SCANNERS,MODEMS,CD_DRIVES);
//
//        printf("\n+++++++++++++++++++++++++++++++++++++++++\n");
}

void deallocateResources(struct proc *realTimeItem){
    
//    printf("\n-----------de allocating resources ---------------\n");
//  printf("available BEFORE DEALLOOCOCOCO printers %d , scanners %d, modems %d ,cd %d\n",PRINTERS,SCANNERS,MODEMS,CD_DRIVES);
    PRINTERS += realTimeItem->printers;
    SCANNERS += realTimeItem->scanners;
    MODEMS += realTimeItem->modems;
    CD_DRIVES += realTimeItem->cds;
//     printf("available AFTER DEALLOOCOCOCO printers %d , scanners %d, modems %d ,cd %d\n",PRINTERS,SCANNERS,MODEMS,CD_DRIVES);
//      printf("\n---------------------------------------------\n");
 
}

int main(void) {
    if(realTime==NULL){
        realTime = malloc(sizeof (struct queue));
    }
    if(priorityQueue1==NULL){
        priorityQueue1 = malloc(sizeof (struct queue));
    }
    
    if(priorityQueue2==NULL){
        priorityQueue2 = malloc(sizeof (struct queue));
    }
    if(priorityQueue3==NULL){
        priorityQueue3 = malloc(sizeof (struct queue));
    }
    
    FILE* file = fopen("dispatchlist", "r");
    char line[256];
    int i =0;
    
    while (fgets(line, sizeof(line), file)) {
        
        int init_size = strlen(line);
        char delim[] = ",";
        
        char *ptr = strtok(line, delim);
        struct proc a;
        
        while(ptr != NULL)
        {
            if(i==0){
                a.arrivalTime = atoi(ptr); i++;
            } else if(i==1){
                a.priority = atoi(ptr); i++;
            } else if(i==2){
                a.processorTime = atoi(ptr);i++;
            }else if(i==3){
                a.memory= atoi(ptr);i++;
            }else if(i==4){
                a.printers = atoi(ptr);i++;
            }else if(i==5){
                a.scanners = atoi(ptr);i++;
            }else if(i==6){
                a.modems = atoi(ptr);i++;
            }else if(i==7){
                a.cds = atoi(ptr);i++;
            }
            
            ptr = strtok(NULL, delim);
        }
        a.pid = -1;
        a.address =0;
        i =0;
        if(a.priority == 0){
//            printf("added real time\n");
            push(realTime,a);
        }
        if(a.priority == 1){
//            printf("added P1\n");
            push(priorityQueue1,a);
        }
        if(a.priority == 2){
//            printf("added P2\n");
            push(priorityQueue2,a);
        }
        if(a.priority == 3){
//            printf("added P3\n");
            push(priorityQueue3,a);
        }
        
        
    }
    
    struct proc *realTimeItem = pop(realTime);
    
//-----------------------------------REAL TIME ---------------------------------------
    
    while(realTimeItem!=NULL){
        int memSize =realTimeItem->memory;
        int startingIndex = findFreeMemoryIndex(memSize,1);
        if(startingIndex!=-1){
            allocateMemory(startingIndex,memSize);
            realTimeItem->address = startingIndex;
            printProc(*realTimeItem);
            executeRealTimeProcess(*realTimeItem,realTimeItem->processorTime);
            
            freeMemory(startingIndex,memSize);
            
        }
        realTimeItem = pop(realTime);
    }
//-----------------------------------Q1 ---------------------------------------
    struct proc *priorityQueue1Item = pop(priorityQueue1);
    
    
    while(priorityQueue1Item!=NULL){
        int status;
        
        int memSize =priorityQueue1Item->memory;
        int startingIndex = findFreeMemoryIndex(memSize,0);
        if(priorityQueue1Item->pid != -1){
            
            runExistingProcess(priorityQueue1Item);
            if(priorityQueue1Item->processorTime!=0){
//                printf("\npushed to queue 2 new proc\n");
                push(priorityQueue2,*priorityQueue1Item);
            } else {
                deallocateResources(priorityQueue1Item);
                freeMemory(priorityQueue1Item->address,memSize);
            }
            
            
        } else if(startingIndex!=-1 && priorityQueue1Item->pid ==-1 && checkResources(priorityQueue1Item)==1){
            allocateResources(priorityQueue1Item);
            allocateMemory(startingIndex,memSize);
            priorityQueue1Item->address = startingIndex;
            printProc(*priorityQueue1Item);
            executeProcess(priorityQueue1Item,1);
            if(priorityQueue1Item->processorTime!=0){
//                printf("\npushed to queue 2 new proc\n");
                push(priorityQueue2,*priorityQueue1Item);
            } else {
                freeMemory(priorityQueue1Item->address,memSize);
                deallocateResources(priorityQueue1Item);
            }
            
            
        } else {
//            printf("\npushed to queue 2\n");
            push(priorityQueue2,*priorityQueue1Item);
        }
        priorityQueue1Item = pop(priorityQueue1);
    }
    //-----------------------------------Q2 ---------------------------------------
    priorityQueue1Item = pop(priorityQueue2);
    
    
    while(priorityQueue1Item!=NULL){
        int status;
        
        int memSize =priorityQueue1Item->memory;
        int startingIndex = findFreeMemoryIndex(memSize,0);
//        printf("Starting Index %d \n", priorityQueue1Item->pid);
        if(priorityQueue1Item->pid != -1){
//            printf("running exisiting\n");
            runExistingProcess(priorityQueue1Item);
            
            if(priorityQueue1Item->processorTime!=0){
//                printf("\npushed to queue 3 new proc\n");
                push(priorityQueue3,*priorityQueue1Item);
            } else {
                freeMemory(priorityQueue1Item->address,memSize);
                deallocateResources(priorityQueue1Item);
            }
        } else if(startingIndex!=-1 && priorityQueue1Item->pid ==-1&& checkResources(priorityQueue1Item)==1 ){
            allocateResources(priorityQueue1Item);
            allocateMemory(startingIndex,memSize);
            priorityQueue1Item->address = startingIndex;
            printProc(*priorityQueue1Item);
            executeProcess(priorityQueue1Item,1);
            if(priorityQueue1Item->processorTime!=0){
//                printf("\npushed to queue 3 new proc\n");
                push(priorityQueue3,*priorityQueue1Item);
            } else {
                freeMemory(priorityQueue1Item->address,memSize);
                deallocateResources(priorityQueue1Item);
            }
            
            
        } else {
//            printf("\npushed to queue 3\n");
            push(priorityQueue3,*priorityQueue1Item);
        }
        priorityQueue1Item = pop(priorityQueue2);
    }
//    printf("\ndone ququeueueue2\n");
    //-----------------------------------Q3---------------------------------------
    priorityQueue1Item = pop(priorityQueue3);
    
    while(priorityQueue1Item!=NULL){
        int status;
        int memSize =priorityQueue1Item->memory;
        int startingIndex = findFreeMemoryIndex(memSize,0);
        if(priorityQueue1Item->pid != -1){
//            printf("running exisiting\n");
            runExistingProcess(priorityQueue1Item);
            if(priorityQueue1Item->processorTime!=0){
                push(priorityQueue3,*priorityQueue1Item);
            } else {
                freeMemory(priorityQueue1Item->address,memSize);
                deallocateResources(priorityQueue1Item);
            }
        } else if(startingIndex!=-1 && priorityQueue1Item->pid ==-1&& checkResources(priorityQueue1Item)==1 ){
            allocateResources(priorityQueue1Item);
//            printf("\n started new procees\n");
            allocateMemory(startingIndex,memSize);
            priorityQueue1Item->address = startingIndex;
            printProc(*priorityQueue1Item);
            executeProcess(priorityQueue1Item,1);
            if(priorityQueue1Item->processorTime!=0){
                push(priorityQueue3,*priorityQueue1Item);
            } else {
                freeMemory(priorityQueue1Item->address,memSize);
                deallocateResources(priorityQueue1Item);
            }
            
            
        } else {
//            printf("\nReadded to q3\n");
            push(priorityQueue3,*priorityQueue1Item);
        }
        priorityQueue1Item = pop(priorityQueue3);
    }
    
    
//    printf("\ndone ququeueueue33333\n");
    
    
}

void sigstp()

{
    signal(SIGTSTP, sigstp);
    //    printf("CHILD: I have received a SIGHUP\n");
}

void sigCont() {
    signal(SIGCONT, sigCont);
    //    printf("CHILD: I have received a SIGCONT\n");
}
void sigint() {
    signal(SIGINT, sigint);
    //    printf("CHILD: I have received a SIGINT\n");
}
