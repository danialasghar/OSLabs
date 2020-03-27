typedef struct proc{
    int arrivalTime;
    int priority;
    int pid;
    int processorTime;
    int memory ;
    int printers;
    int scanners ;
    int modems;
    int cds;
    int address;
}proc;

typedef struct queue {
    struct proc process;
    struct queue *next;
    
} queue;

void push(struct queue *q1,struct proc process);

struct proc *pop(struct queue *q1);
