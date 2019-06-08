#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "shellmemory.h"
#include "shell.h"

// errcodes:
// 0=help,set,get,script
// 1=proble,
// 2=quit
int errCode = 0;

int help() {
	printf("COMMAND                 DESCRIPTION \n");
	printf("help                   Displays all the commands \n");
	printf("quit                   Exits / terminates the shell with Bye!\n");
	printf("set VAR STRING         Assigns a value to shell memory \n");
	printf("print VAR              Prints the STRING assigned to VAR \n");
	printf("run SCRIPT.TXT         Executes the file SCRIPT.TXT \n");
	return 0;
}

int quit(){
	return -1;
}

int set(char *words[]) {
	insert(words[1],words[2]);

	return 0;
}

int print(char *words[]) {
	printf(getVal(words[1]),"%s");
	printf("\n");
	return 0;
}

//does not work
int script(char *words[]) {

	int errCode = 0;
	char line[1000];
	FILE *p = fopen(words[1],"rt");  // the program is in a file
	if (!p){
		printf("script not found \n");
		return errCode;
	}
	fgets(line,999,p);
	while(!feof(p)) {
        //needed to fix space bug
		int len = strlen(line);
		line[len-1] = '\0' ;

		if (errCode == -1) {
			fclose(p);
			return errCode;
		}
		errCode = parse(line);
		if (errCode == -1) {
			fclose(p);
			return errCode;
		}
		if (errCode != 0) {
			printf("command error");
		}
		


		fgets(line,999,p);
	}

	fclose(p);
	return errCode;
}

int interpreter(char *words[]) {  // assumes: cmd switchs args
								  // assumes: words[0] is cmd


	// The user is asking to execute a single command
	if (!strcmp(words[0], "help")){
		errCode = help();
	}
	else if (!strcmp(words[0], "quit")) {
		errCode = quit();
	}
	else if (!strcmp(words[0], "set"))  {
		errCode = set(words);
	}
	else if (!strcmp(words[0], "print")) {
		errCode = print(words);
	}
	else if (!strcmp(words[0], "run")) {
	
		errCode = script(words);
	}

	else errCode = 1;


	return errCode;
}



