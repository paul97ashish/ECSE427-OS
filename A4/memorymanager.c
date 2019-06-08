#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "ram.h"
#include "pcb.h"
#include "kernel.h"

int launcher(FILE *p);
int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
int findFrame(FILE *page);
int updateFrame(int frameNumber, int victimFrame, FILE *page);
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);
int notInPageTable(int pageNumber, struct PCB *p);
int findVictim(struct PCB *p);
char *copyToBacking(FILE *p);

int notInPageTable(int pageNumber, struct PCB *p);

int filesCount = 1;

int launcher(FILE *p){
    char *fileName = copyToBacking(p); 
    char fileToOpen[80];
    strcpy(fileToOpen, "BackingStore/");
    strcat(fileToOpen, fileName);
    FILE *current_file = fopen(fileToOpen, "r");

    if(!current_file)
        return 0;
    
    struct PCB *temp = makePCB(current_file); 

    if(!temp)
        return 0;

    addToReady(temp);
    
    
    
    int pagesNum = countTotalPages(current_file);


    for(int x = 0; x<2; x++){
        if(x == pagesNum)
            break;
    
        int ramIdx = findFrame(current_file);
        
        updateFrame(ramIdx, -999, findPage(x, current_file));
        updatePageTable(temp, x, ramIdx, -999);

    }
    programs_in_ram++;
    return 1;
}

char *copyToBacking(FILE *p){
    static char fileName[80];
    char fileNumber[10];
    sprintf(fileNumber, "%d", filesCount++); 
    strcpy(fileName, "process");
    strcat(fileName, fileNumber);
    strcat(fileName, ".txt");
    
    FILE *newFile = fopen(fileName, "w");

    char ch;
   while ((ch = fgetc(p)) != EOF)
      fputc(ch, newFile);
    
    fclose(p);
    fclose(newFile);

    char copyCommand[80];
    strcpy(copyCommand, "mv ");
    strcat(copyCommand, fileName);
    strcat(copyCommand, " BackingStore/");
    system(copyCommand);
    return fileName;
}

int countTotalPages(FILE *f){
    int count = 1;
    int ch = 0;
    while(!feof(f)){
        ch = fgetc(f);
        if(ch == '\n'){
            count++;
        }
    }

    rewind(f);
    
    if(count%4 != 0)
        return ((count/4) + 1);
    else
        return (count/4);
    
}

FILE *findPage(int pageNumber, FILE *f){
    rewind(f);
    FILE *fp2 = fdopen(dup(fileno(f)), "r");
    rewind(f);
    rewind(fp2);
    char line[1000];
    for(int x = 0; x<pageNumber*4; x++)
        fgets(line, sizeof(line), fp2);
    
    rewind(f);
    return fp2;
}

int findFrame(FILE *page){
    for(int x = 0; x<10; x++){
        if (ram[x] == NULL)
            return x;
    }
    return -1;
}

int findVictim(struct PCB *p){
    srand(time(0));
    int ran = rand() % 10;
    if(notInPageTable(ran, p))
        return ran;
    else
        while(!notInPageTable((++ran)%10, p));
    
    return ran;
}


int notInPageTable(int pageNumber, struct PCB *p){
    if(p->pageTable[pageNumber] != 100) 
        return 0;
    else
        return 1;
}

int updateFrame(int frameNumber, int victimFrame, FILE *page){
    if(frameNumber == -1)
        ram[victimFrame] = page;
    else
        ram[frameNumber] = page;
    
    return frameNumber;
}

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame){
    if(victimFrame == -1)
        p->pageTable[pageNumber] = 100;    
    if(frameNumber == -1)
        p->pageTable[pageNumber] = victimFrame;
    else 
        p->pageTable[pageNumber] = frameNumber;
    
    return frameNumber;
}