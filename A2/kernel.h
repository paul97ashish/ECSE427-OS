
void myInit(FILE *p);
void scheduler();
struct CPU *cpu;
int fileAvailable;

struct QNode 
{ 
    struct PCB *key; 
    struct QNode *next; 
}; 

struct Queue 
{ 
    struct QNode *front; 
    struct QNode *rear;
}; 

struct QNode* newNode(struct PCB *k);
struct Queue *createQueue();
struct QNode *deQueue(struct Queue *q);
struct Queue *readyQueue;

void enQueue(struct Queue *q, struct PCB *k);
