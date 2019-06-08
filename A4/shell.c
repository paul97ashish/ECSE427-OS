#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "interpreter.h"

int parseInput(char ui[], int mode);
int shell(void);

int shell(void){
    char prompt[] = "$";

    char userInput[1000];
    int errorCode = 0;
    int textFinished = 1;
    int s;
    while(1){
        printf("%s ", prompt);
        fgets(userInput, 999, stdin);

        if(feof(stdin)){// means we reached the last command
            textFinished = 0;
        }
        
         s = parseInput(userInput, 0);
        
        //Error handling
        if(s == 1){
            printf("Command doesn't exist\n");
        }
        if (s == 8){
            printf("Wrong number/format of arguments\n");
        }
        if (s == 9){
            printf("File doesn't exist\n");
        }

        if(textFinished == 0){//go back to terminal if textfile is finished
            freopen("/dev/tty", "r", stdin);
        }
    }
    return 0;
}

int parseInput(char ui[], int mode){
    int w = 0;
    char *words[1000];
    char temp[1000];

    //strcpy modifies the string, so we need to copy it
    strcpy(temp, ui);

    //pointer to first word
    char* token = strtok(temp, " \n\t");

    //loop through the tokens
    while (token) {
        //add token to words array
        words[w] = strdup(token);
        token = strtok(NULL," ");
        w++;
    }

    //strtok adds a space to the last word so we have to overwrite it
    if(isspace(words[w-1][strlen(words[w-1])-1])){
        words[w-1][strlen(words[w-1])-1] = 0;
    }

    return interpreter(words, w);
}
