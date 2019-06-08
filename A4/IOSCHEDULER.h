#include "pcb.h"

struct SNode 
{ 
    char* key; 
    struct SNode *next; 
}; 
struct SQueue 
{ 
    struct SNode *front, *rear; 
}; 

char *IOscheduler(char *data, struct PCB *ptr, int cmd);

int ioFile(int file);

struct SQueue *createSQueue();

struct SQueue *ioQueue;