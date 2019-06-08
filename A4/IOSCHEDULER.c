#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DISK_driver.h"
#include "IOSCHEDULER.h"

char *IOscheduler(char *data, struct PCB *ptr, int cmd);
void enSQueue(struct SQueue *q, char* k);
struct SNode *deSQueue(struct SQueue *q);
struct SQueue *ioQueue;

int currentFile;

char *IOscheduler(char *data, struct PCB *ptr, int cmd){
    
    enSQueue(ioQueue, data);

    char *toReturn = (char*) malloc(100*sizeof(char));

    while(1){
        struct SNode *temp = deSQueue(ioQueue);

        if(cmd == 0){
            int read = readBlock(currentFile); 
            if(read == -1) return NULL;

            strcpy(toReturn, returnBlock());
        }
    
        if (cmd == 1){
            writeBlock(currentFile, data);
            strcpy(toReturn, data);
        }

        temp = deSQueue(ioQueue);
        if(!temp) break;
    }

    return toReturn;
    
}
  
struct SNode* newNodeS(char* k) 
{ 
    struct SNode *temp = (struct SNode*)malloc(sizeof(struct SNode)); 
    temp->key = k; 
    temp->next = NULL; 
    return temp;  
} 
  
struct SQueue *createSQueue() 
{ 
    struct SQueue *q = (struct SQueue*)malloc(sizeof(struct SQueue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
void enSQueue(struct SQueue *q, char* k) 
{ 
    struct SNode *temp = newNodeS(k); 
      if (q->rear == NULL) 
    { 
       q->front = q->rear = temp; 
       return; 
    } 
  
    q->rear->next = temp; 
    q->rear = temp; 
} 
  
struct SNode *deSQueue(struct SQueue *q) 
{ 
    if (q->front == NULL) 
       return NULL; 
  
    struct SNode *temp = q->front; 
    q->front = q->front->next; 
  
    if (q->front == NULL) 
       q->rear = NULL; 
    return temp; 
} 


int ioFile(int file){
    currentFile = file;
    return currentFile;
}