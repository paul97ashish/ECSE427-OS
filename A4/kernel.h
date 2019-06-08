#include <stdio.h>
extern FILE *ram[10];
int programs_in_ram;

void myinit(FILE *p);
void addToReady(struct PCB *pcb);
void scheduler();
void kernelPageFault();
struct CPU *cpu;
struct PCB *temp_pcb;
int fileAvailable;

struct QNode* newNode(struct PCB *k);
struct Queue *createQueue();
void enQueue(struct Queue *q, struct PCB *k);
struct QNode *deQueue(struct Queue *q);
struct Queue *readyQueue;

struct QNode 
{ 
    struct PCB *key; 
    struct QNode *next; 
}; 

struct Queue 
{ 
    struct QNode *front, *rear; 
}; 