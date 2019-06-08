#include <stdio.h>
#include <stdlib.h>
#include "ram.h"
#include "pcb.h"

struct PCB* makePCB(FILE *p);
int countPages(FILE *f);

struct PCB* makePCB(FILE *p) {
    struct PCB *pcb_instance = (struct PCB*)malloc(sizeof(struct PCB));
    pcb_instance->PC = p;
    pcb_instance->pages_max = countPages(p);
    pcb_instance->PC_page = 0;
    pcb_instance->PC_offset = 0;
    for(int x = 0; x<10; x++)
        pcb_instance->pageTable[x] = 100;
    return pcb_instance;
}

int countPages(FILE *f){
    int count = 1;
    int ch = 0;
    while(!feof(f)){
        ch = fgetc(f);
        if(ch == '\n'){
            count++;
        }
    }

    rewind(f);
    
    if(count%4 != 0){
        return ((count/4) + 1);
    }else{
        return (count/4);
    }
}