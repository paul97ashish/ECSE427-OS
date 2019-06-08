#include <stdlib.h>
#include <stdio.h>
#include "ram.h"
#include "kernel.h"
#include "cpu.h"
#include "pcb.h"
#include "shell.h"


struct CPU *cpu;
void scheduler();
void addToRAM(FILE *p);
void addToReady(struct PCB *pcb);
void initShellMemory();
struct PCB* makePCB(FILE *p);


int main(void) {
	char buffer[1000], buf0[100], buf1[100], buf2[100];
	int done = 0, result = 0;

	initShellMemory();

	printf("Kernel 1.0 loaded!\n");
	printf("Welcome to the Ashish Paul shell!\n");
	printf("Shell Version 2.0 Created Febuary 2019\n");
	readyQueue = createQueue();

	while(!done) {
		printf("$ ");
		fgets(buffer,999, stdin);
		
		result = prompt(buffer);

		if (result == 99) done = 1;
	}

	printf("Good bye.\n");
	return 0;
}

void addToReady(struct PCB *pcb){
	    enQueue(readyQueue, pcb);
}

void myInit(FILE *p){
	addToRAM(p);
	struct PCB *tempPcb = makePCB(p);
	addToReady(tempPcb);	
}


void scheduler(){
    cpu = (struct CPU*)malloc(sizeof(struct CPU));
    cpu->quanta = 2;
    struct QNode *temp;

    if(numOfProgs)
        temp = deQueue(readyQueue);
    
    struct PCB *tempPcb;
    
    while(numOfProgs){
        tempPcb = temp->key;
        cpu->IP = tempPcb->PC;
        fileAvailable = 1; 
        CPUrun(cpu->quanta);
        
        if(fileAvailable){
            enQueue(readyQueue, tempPcb);
        }else{
            ram[tempPcb->ramInd] = NULL;
            numOfProgs--;
            free(tempPcb);
        }
        
        temp = deQueue(readyQueue);
        if(!temp){
            break;
        }
    }
}
struct QNode* newNode(struct PCB *k) 
{ 
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->key = k; 
    temp->next = NULL; 
    return temp;  
} 
  
struct Queue *createQueue() 
{ 
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
void enQueue(struct Queue *q, struct PCB *k) 
{ 
    struct QNode *temp = newNode(k); 
  
    if (q->rear == NULL) 
    { 
       q->front = q->rear = temp; 
       return; 
    } 
  
    q->rear->next = temp; 
    q->rear = temp; 
} 
  
struct QNode *deQueue(struct Queue *q) 
{ 
    if (q->front == NULL) 
       return NULL; 
  
    struct QNode *temp = q->front; 
    q->front = q->front->next; 
  
    if (q->front == NULL) 
       q->rear = NULL; 
    return temp; 
} 