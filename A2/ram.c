#include <stdio.h>


FILE *ram[10];
int numOfProgs=0;

void addToRAM(FILE *p){
	 
	if(numOfProgs==10){
		printf("The ram is full ,\n");
	}
	ram[numOfProgs] = p;
	numOfProgs++;
}