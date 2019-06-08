#include <stdio.h>

struct PCB {
    FILE *PC;
    int pageTable[10];
    int PC_page, PC_offset, pages_max;
};

struct PCB* makePCB(FILE *p);