#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcb.h"
#include "ram.h"

struct PCB  *makePCB(FILE *p){
	struct PCB *pcbInstance = (struct PCB*)malloc(sizeof(struct PCB));
    pcbInstance->PC = p;
    pcbInstance->ramInd = numOfProgs-1;
    return pcbInstance;
}
