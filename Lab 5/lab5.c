#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3

int available[NUM_RESOURCES];
pthread_mutex_t lock;

int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

int allocation[NUM_CUSTOMERS][NUM_RESOURCES];
int need[NUM_CUSTOMERS][NUM_RESOURCES];

bool request_res(int n_customer, int request[]);
bool release_res(int n_customer, int release[]);
bool is_Safe(int tempAvalible[],int tempNeed[NUM_CUSTOMERS][NUM_RESOURCES],int tempAllocation[NUM_CUSTOMERS][NUM_RESOURCES]);

typedef struct {
    int custNo;
    int request[3];
} CustomerData;

bool request_res(int n_customer, int request[]){
    
    pthread_mutex_lock(&lock);
    
    int tempAvailable[NUM_RESOURCES];
    
    int tempMaximum[NUM_CUSTOMERS][NUM_RESOURCES];
    
    int tempAllocation[NUM_CUSTOMERS][NUM_RESOURCES];
    int tempNeed[NUM_CUSTOMERS][NUM_RESOURCES];
    
    for(int i = 0; i<NUM_RESOURCES;i++){
        tempAvailable[i] = available[i];
    }
    
    
    for(int i =0;i<NUM_CUSTOMERS;i++){
        for(int j =0;j<NUM_RESOURCES;j++){
            
            tempMaximum[i][j] = maximum[i][j];
            tempAllocation[i][j] = allocation[i][j];
            tempNeed[i][j] = need[i][j];
            
        }
        
    }
    
    
    
    for(int i =0;i<NUM_RESOURCES;i++){
        tempAvailable[i] -= request[i];
        tempAllocation[n_customer][i] += request[i];
        tempNeed[n_customer][i] = tempMaximum[n_customer][i]-tempAllocation[n_customer][i];
    }
    
    if(is_Safe(tempAvailable,tempNeed,tempAllocation)==true){
        for(int i =0;i<NUM_RESOURCES;i++){
            available[i] -= request[i];
            allocation[n_customer][i] += request[i];
            need[n_customer][i] = maximum[n_customer][i]-allocation[n_customer][i];
        }
        
        int res[3];
        
        bool canDealloc = true;
        
        for(int i =0;i<NUM_RESOURCES;i++){
            if(allocation[n_customer][i]<maximum[n_customer][i]){
                canDealloc = false;
            }
            res[i] = allocation[n_customer][i];
               
        }
        if(canDealloc==true){
            if(release_res(n_customer,res)==true){
                printf("successfully dealloced");
            } else{
                printf("error in dealloced");
            }
        }
        pthread_mutex_unlock(&lock);
        
        return true;
        
    } else {
        pthread_mutex_unlock(&lock);
        return false;
    }
    
    
    
    
    
}

bool release_res(int n_customer, int release[]){
//    pthread_mutex_lock(&lock);
    int tempAvailable[NUM_RESOURCES];
    
    int tempMaximum[NUM_CUSTOMERS][NUM_RESOURCES];
    
    int tempAllocation[NUM_CUSTOMERS][NUM_RESOURCES];
    int tempNeed[NUM_CUSTOMERS][NUM_RESOURCES];
    
    for(int i = 0; i<NUM_RESOURCES;i++){
        tempAvailable[i] = available[i];
    }
    
    
    for(int i =0;i<NUM_CUSTOMERS;i++){
        for(int j =0;j<NUM_RESOURCES;j++){
            
            tempMaximum[i][j] = maximum[i][j];
            tempAllocation[i][j] = allocation[i][j];
            tempNeed[i][j] = need[i][j];
            
        }
        
    }
    for(int i =0;i<NUM_RESOURCES;i++){
        tempAvailable[i] += release[i];
        tempAllocation[n_customer][i] -= release[i];
        tempNeed[n_customer][i] = tempMaximum[n_customer][i]-tempAllocation[n_customer][i];
    }
    
    if(is_Safe(tempAvailable,tempNeed,tempAllocation)==true){
        for(int i =0;i<NUM_RESOURCES;i++){
            available[i] += release[i];
            allocation[n_customer][i] -= release[i];
            need[n_customer][i] = maximum[n_customer][i]-allocation[n_customer][i];
        }
//        pthread_mutex_unlock(&lock);
        return true;
        
    } else {
//        pthread_mutex_unlock(&lock);
        return false;
    }
}

bool is_Safe(int tempAvalible[],int tempNeed[NUM_CUSTOMERS][NUM_RESOURCES],int tempAllocation[NUM_CUSTOMERS][NUM_RESOURCES]) {
    
    bool finish[NUM_CUSTOMERS] = {false};
    
    bool keepChecking = true;
    int finishedCount = 0;
    int notFinishedCount =0;
    while(keepChecking){
        
        
        for(int i =0;i<NUM_CUSTOMERS;i++){
            bool canFinish = true ;
            
            for(int j =0;j<NUM_RESOURCES;j++){
                if(finish[i]==false && tempNeed[i][j]>tempAvalible[j]){
                    //                     printf("customer %d cannnnott finish\n",i);
                    canFinish = false;
                    break;
                }
            }
            if(canFinish==true){
                //                printf("customer %d can finish\n",i);
                notFinishedCount = 0;
                finishedCount++;
                finish[i] = true;
                for(int k =0;k<NUM_RESOURCES;k++){
                    tempAvalible[k] += tempAllocation[i][k];
                    
                }
            } else {
                finishedCount =0;
                notFinishedCount++;
            }
            if(notFinishedCount==NUM_CUSTOMERS || finishedCount==NUM_CUSTOMERS){
                keepChecking = false;
            }
            
        }
    }
    
    bool isSafe = true;
    
    for(int i =0;i<NUM_CUSTOMERS;i++){
        if(finish[i]==false){
            isSafe = false;
        }
    }
    
    return isSafe;
}

void* handleReq(void* s) {
    CustomerData *params =  (CustomerData*)s;
//    for(int j =0;j<NUM_RESOURCES;j++){
//
//        printf("request %d \n",params->request[j]);
//    }
    
    if(request_res(params->custNo,params->request)==true){
        printf("success cust no %d\n",params->custNo);
    } else {
           printf("fail cust no %d\n",params->custNo);
    }
    
}

int main(int argc, char *argv[]) {
    
    
    for(int i = 1; i<argc;i++){
        available[i-1] =atoi(argv[i]);
    }
    
    
    
    int   maximum1[NUM_CUSTOMERS][NUM_RESOURCES] ={{7,5,3},
        {3,2,2},
        {9,0,2},
        {2,2,2},
        {4,3,3}};
    int need1[NUM_CUSTOMERS][NUM_RESOURCES] = {{7,4,3},
        {1,2,2},
        {6,0,0},
        {0,1,1},
        {4,3,1}};
    int allocation1[NUM_CUSTOMERS][NUM_RESOURCES] = {{0,1,0},
        {2,0,0},
        {3,0,2},
        {2,1,1},
        {0,0,2}};
    for(int i =0;i<NUM_CUSTOMERS;i++){
        for(int j =0;j<NUM_RESOURCES;j++){
            
            maximum[i][j] = maximum1[i][j];
            allocation[i][j] = allocation1[i][j];
            need[i][j] = need1[i][j];
            
            
        }
        
    }
    
    int request1[3] = {1,0,2};

    
    pthread_t tid_customer[NUM_CUSTOMERS];
    int r[3] = {1,0,2};
    for(int i =0;i<NUM_CUSTOMERS;i++){

        
        CustomerData *data = (CustomerData *) malloc(sizeof(CustomerData));
        data->custNo = i;
        for(int j =0;j<NUM_RESOURCES;j++){
          
            data->request[j] = r[j];
        }
        
        pthread_create(&tid_customer[i], NULL, handleReq, (void *)data);
    }
    
    for (int i = 0; i < NUM_CUSTOMERS; i++){
        pthread_join(tid_customer[i], NULL);
    }
    
}
