#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "shellmemory.h"
#include "interpreter.h"

int parse(char ui[]){

	char tmp[200];
	char *words[100];
	int a,b;
	int w=0;
	//removes the spaces in the beginning
	for(a=0; ui[a]==' ' && a<100; a++);

	//
	while(ui[a] != '\0' && a<1000) { 
		for(b=0; ui[a]!='\0' && ui[a]!=' ' && a<1000; a++, b++){
			tmp[b] = ui[a];         //extracts a word
		}
		tmp[b] = '\0';              // end that string

		words[w] = strdup(tmp);		// add the word to the word array

		a++;
		w++;
	}
	int vari=interpreter(words);
	
	return vari; // assume: cmd switches args
}

int main(){
	char prompt[100] = {'$','\0'};  // prompts can be modified
	char userInput[1000]; //users input stored here
	printf("Welcome to the Ashish shell!\n");
	printf("Version 1.0 Created January 2019\n");
	int errorCode=0; // 0 is no error
	int cmdntfound=0;
	while(1){
		
		printf("%s",prompt);
		fgets(userInput, 999, stdin);// limit input to array size
		
		//needed to fix space bug	
		int len = strlen(userInput);
		userInput[len-1] = '\0' ;

		errorCode = parse(userInput);
		if(errorCode == -1){
			printf("Bye!\n");
			exit(0);
		}
		if(errorCode == 1){
			printf("Command not found \n");
			cmdntfound++;
			if(cmdntfound==10){
			printf("Either no quit command or no enter at the end of .txt file");
			exit(0);
			}
		}
		userInput[0]='\0';
	}
}

