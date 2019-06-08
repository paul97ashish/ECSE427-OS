#include <stdio.h>
#include <string.h>

#define MAX_ENTRIES 100
char *memory[MAX_ENTRIES][2];
int entries = 0;

int checkVar(char var[]);
int setVar(char var[], char value[]);
void printVar(char var[]);

int setVar(char var[], char value[]) {
    if(entries == MAX_ENTRIES){
        return 0;
    }
    printf(" %s\n", value);
    int varLocation = checkVar(var);

    if(varLocation == -1){
        entries++;
        memory[entries][0] = var;
        memory[entries][1] = value;
        return 1;
    }else{
        memory[varLocation][1] = strdup(value);
        return 1;
    }
}
int checkVar(char var[]) {
    int x;
    for(x=0; x<entries+1; x++){
        if(memory[x][0] != NULL && strcmp(var, memory[x][0])==0){
            return x;
        }
    }
    return -1;
}


void printVar(char var[]){
    int varLocation = checkVar(var);
    if(varLocation == -1){
        printf("VAR doesn't exist\n");
    }else{
        printf("Value of Var: %s is %s\n", var, memory[varLocation][1]);
    }
}