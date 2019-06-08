#include <stdio.h>

void runCPU(int quanta);

struct CPU {
    FILE *IP; 
    int offset;
    char IR[1000];
    int quanta; 
};