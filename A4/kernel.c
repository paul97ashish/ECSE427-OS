#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "DISK_driver.h"
#include "IOSCHEDULER.h"
#include "ram.h"
#include "shell.h"
#include "cpu.h"
#include "memorymanager.h"
#include "kernel.h"

FILE *ram[10];
int programs_in_ram = 0;

void myinit(FILE *p);
void addToReady(struct PCB *pcb);
void scheduler();
void boot();
void kernelPageFault(); 
struct PCB *findVictimPCB(FILE *f);

struct CPU *cpu;
struct PCB *temp_pcb;
struct PCB *available_pcbs[10];
int total_pcbs = 0;


int fileAvailable;
int pageAvailable;

struct QNode* newNode(struct PCB *k);
struct Queue *createQueue();
void enQueue(struct Queue *q, struct PCB *k);
struct QNode *deQueue(struct Queue *q);
struct Queue *readyQueue;
void addToRam(FILE *p); 


int main(void){
    initIO();

    boot();
    printf("\nKernel 1.0 loaded!\n");
    printf("Welcome to the Ashish shell\n");
    printf("Version 4.0 Created April 2019\n");
    ioQueue = createSQueue(); 
    readyQueue = createQueue();
    shell();
    return 0;
}

void boot(){
    for(int i = 0; i < 10; i++)
        ram[i] = NULL;

    DIR* dir = opendir("BackingStore");

    if(dir){
        system("rm -rf BackingStore/ && mkdir BackingStore");
    }else if(ENOENT == errno){
        system("mkdir BackingStore");
    }else{
        printf("DIRECTORY ERROR\n");
    }
}


void addToReady(struct PCB *pcb){
    available_pcbs[total_pcbs++] = pcb;
    enQueue(readyQueue, pcb);
}

void scheduler(){
    cpu = (struct CPU*)malloc(sizeof(struct CPU));
    cpu->quanta = 2;
    
    struct QNode *temp;
    if(programs_in_ram)
        temp = deQueue(readyQueue);
    
    while(programs_in_ram && temp){
        temp_pcb = temp->key;
        cpu->IP = temp_pcb->PC;

        cpu->offset = temp_pcb->PC_offset;

        fileAvailable = 1; 
        runCPU(cpu->quanta); 
        
        if(fileAvailable){
            enQueue(readyQueue, temp_pcb);
        }else{
            programs_in_ram--;
            fclose(temp_pcb->PC);
            free(temp_pcb);
        }
        
        temp = deQueue(readyQueue);
        if(!temp){
            break;
        }
    }
}

void kernelPageFault(){
    temp_pcb->PC_page++;
    if(temp_pcb->PC_page > temp_pcb->pages_max-1){
        fileAvailable = 0;
        return;
    }else{
        if(!notInPageTable(temp_pcb->PC_page, temp_pcb)){
            temp_pcb->PC_offset = 0; 
            temp_pcb->PC = ram[temp_pcb->pageTable[temp_pcb->PC_page]];
        }else{
            temp_pcb->PC_offset = 0;
            FILE *tempPage = findPage(temp_pcb->PC_page, temp_pcb->PC);
            int frameNumber = findFrame(tempPage);

            if(frameNumber == -1){
                int victimFrame = findVictim(temp_pcb);
                FILE *victimFile = ram[victimFrame];

                updateFrame(frameNumber, victimFrame, tempPage);
                updatePageTable(temp_pcb, temp_pcb->PC_page, victimFrame, -999);

                temp_pcb->PC = ram[temp_pcb->pageTable[temp_pcb->PC_page]];
                struct PCB *victimPCB = findVictimPCB(victimFile);
                if(victimPCB)
                    updatePageTable(victimPCB, victimFrame, 100, -1);
                  
            }else{
                updateFrame(frameNumber, -999, tempPage); 
                updatePageTable(temp_pcb, temp_pcb->PC_page, frameNumber, -999);

                temp_pcb->PC_offset = 0; 
                temp_pcb->PC = ram[temp_pcb->pageTable[temp_pcb->PC_page]]; 
            }
        }
    }
}


struct PCB* findVictimPCB(FILE *f){
    for(int x = 0; x<total_pcbs; x++){
        if(available_pcbs[x]->PC == f){
            return available_pcbs[x];
        }
    }
    return NULL;
}


void addToRam(FILE *p){
    ram[programs_in_ram++] = p;
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