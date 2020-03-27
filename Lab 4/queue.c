#include "queue.h"
#include "utility.h"

#include <stddef.h>


void push(struct queue *q1,struct proc process) {
    // if(q1==NULL){
    //     q1 = malloc(sizeof (struct queue));
    // }
    struct queue *head = q1;
    while(head->next!=NULL){
        head = head->next;
        
    }
    struct queue *newNode = malloc(sizeof (struct queue));
    newNode->process = process;
    
    head->next = newNode;
    
    
}

struct proc *pop(struct queue *q1) {
    if(q1==NULL || q1->next == NULL){
        return NULL;
    }
    
    struct queue *procPop = q1->next;
    
    q1->next = q1->next->next;
    return &procPop->process;
}
