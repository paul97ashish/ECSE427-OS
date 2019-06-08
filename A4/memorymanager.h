#include <stdio.h>

int launcher(FILE *p);
int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
int findFrame(FILE *page);
int updateFrame(int frameNumber, int victimFrame, FILE *page);
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);
int notInPageTable(int pageNumber, struct PCB *p);
int findVictim(struct PCB *p);