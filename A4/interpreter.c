#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#include "shellmemory.h"
#include "DISK_driver.h"
#include "IOSCHEDULER.h"
#include "shell.h"
#include "ram.h"
#include "cpu.h"
#include "kernel.h"
#include "memorymanager.h"


int interpreter(char *words[], int wordCount);

int help(void);
int quit(void);
int set(char *words[]);
int print(char *words[]);
int run(char *words[]);
int exec(char *words[]);
int mountInt(char *words[]);
int writeInt(char *words[]);
int readInt(char *words[]);

int scriptExists(char *words[], char script[], int counter);

const int WRONG_ARG_NUM_CODE = 8;
const int WRONG_FILE_CODE = 9;
const int SUCCESS_CODE = 12;
const int IO_ERROR = 7;

int word_count;

int interpreter(char *words[], int wordCount){
    int errorCode = 0;
    word_count = wordCount;
    int x;

    if (strcmp(words[0], "help") == 0) {
        if(wordCount == 1){
            errorCode = help();
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }
    else if (strcmp(words[0], "quit") == 0) {
        if(wordCount == 1){
            errorCode = quit();
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }
    else if (strcmp(words[0], "set") == 0) {
        if(wordCount == 3){
             errorCode = set(words);
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }
    else if (strcmp(words[0], "print") == 0) {
        if(wordCount == 2){
            errorCode = print(words);
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }
    else if (strcmp(words[0], "run") == 0) {
        if(wordCount == 2){
            errorCode = run(words);
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }
    else if (strcmp(words[0], "exec") == 0) {
        if(wordCount >= 2){
            errorCode = exec(words);
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }else if (strcmp(words[0], "mount") == 0) {
        if(wordCount == 4){
            errorCode = mountInt(words);
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }else if (strcmp(words[0], "read") == 0) {
        if(wordCount == 3){
            errorCode = readInt(words);
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }else if (strcmp(words[0], "write") == 0) {
        if(wordCount >= 3){
            errorCode = writeInt(words);
        }else{
            errorCode = WRONG_ARG_NUM_CODE;
        }
    }
    else {
        errorCode = 1;
    }
    if(errorCode == 1){
        printf("The Command entered doesn't exist\n");
    }
    if (errorCode == 8){
        printf("Wrong format of arguments\n");
    }
    if (errorCode == 9){
        printf("File does not exist\n");
    }
    if (errorCode == 7)
        printf("Error with the IO system\n");

    return errorCode;
}

int help(){
    printf("Commands \t\t Description\n");
    printf("help \t\t show all commands\n");
    printf("quit \t\t exit shell\n");
    printf("set \t\t assign a value to shell memory\n");
    printf("print \t\t print the string assigned to a variable\n");
    printf("run \t\t execute a script file\n");
    printf("exec \t\t executes concurrent programs\n");
    printf("mount \t\t Mount file in partition\n");
    printf("read \t\t Read from a file\n");
    printf("write \t\t Write to a file\n");
    return SUCCESS_CODE;
}

int quit(){
    printf("\nGOODBYE!\n");
    closeFiles();
    exit(0);
    return SUCCESS_CODE;
}

int set(char *words[]){
    if(setVar(words[1], words[2])){
        printf(" (%s) is set to: (%s)\n", words[1], words[2]);
    }else{
        printf("Max entries has been exceeded\n");
        printf("Variable could not be set\n");
    }
    return SUCCESS_CODE;
}
int print(char *words[]){
    printVar(words[1]);
    return SUCCESS_CODE;
}
int mountInt(char *words[]){
    int blocks = strtol(words[2], NULL, 10);
    int blockSize = strtol(words[3], NULL, 10);

    char partitionName[80];
    strcpy(partitionName, "PARTITION/");strcat(partitionName, words[1]);strcat(partitionName, ".txt");

    FILE *parti = fopen(partitionName, "r");
    
    if(parti){
        fclose(parti);
        if(!mount(words[1]))
            return IO_ERROR; 
    }else{
        if(!partition(words[1], blocks, blockSize))
            return IO_ERROR;
        
        if(!mount(words[1]))
            return IO_ERROR;
    }

    return SUCCESS_CODE;
}
int writeInt(char *words[]){

    char sentence[100]; strcpy(sentence, "");
    for(int x = 2; x<word_count; x++){
        strcat(sentence, words[x]);
        if(x != word_count-1)
            strcat(sentence, " ");
    }
    sentence[99] = '\0';


    int openFile = isFileOpen(words[1]); 
    if(openFile == -1 && partitionMounted != 0)
        openFile = openfile(words[1]);
    
    ioFile(openFile);

    char *written = IOscheduler(sentence, temp_pcb, 1);

    return SUCCESS_CODE;
}
int readInt(char *words[]){
    int openFile = isFileOpen(words[1]);
    printf("openFIle number %d\n", openFile);
    if(openFile == -1 && partitionMounted != 0)
        openFile = openfile(words[1]);

    ioFile(openFile);
    char *buffer = IOscheduler(NULL, temp_pcb, 0);

    if(!buffer){
        printf("ERROR: File is Unavailable\n");
        return IO_ERROR;
    }
    
    char *toSet = (char*) malloc(100*sizeof(char));

    strcpy(toSet, buffer);
    setVar(words[2], toSet);

    return SUCCESS_CODE;
}
int exec(char *words[]) {    
    if(word_count > 11)

    for(int x = 1; x<word_count && x<=10; x++){
        FILE *fp = fopen(words[x], "r");
        if(fp && programs_in_ram <= 10){
            launcher(fp);
        }else{
            printf("File %s does not exit\n", words[x]);
        }
    }
    scheduler();

    for(int x = 0; x<10; x++)
        ram[x] = NULL;
    
    sleep(3);
    system("rm BackingStore/*");

    return SUCCESS_CODE;
}
int run(char *words[]){

    int errorCode = 0;
    char line[1000];
    int lines = 1;
    int ch = 0;
    int lineCounter = 1;

    printf("FileName: %s\n", words[1]);
    FILE *fp = fopen(words[1], "r");
    if(!fp){
        errorCode = WRONG_FILE_CODE;
        return errorCode;
    }

    while(!feof(fp)){
        ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
    fclose(fp);

    FILE *file = fopen(words[1], "r");

    while (fgets(line, sizeof(line), file)) {
        errorCode = parseInput(line, 1);
        lineCounter++;
    }
    fclose(file);
    return errorCode;
}