#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "shellmemory.h"
#include "shell.h"
#include "ram.h"
#include "kernel.h"

int word_count = 4;

int run(char *filename) {
	FILE *ptr;
	char buffer[1000], buf0[100], buf1[100], buf2[100];
	int result = 0;

	ptr = fopen(filename,"rt");
	if (ptr == NULL) return 4; // file not found

	fgets(buffer,999,ptr);
	while(!feof(ptr)) {
		if (strlen(buffer)>1) result = prompt(buffer);
		if (result == 99) break;
		fgets(buffer,999,ptr);
	}

	fclose(ptr);

	if (result == 99) return 99;

	return 0;
}



int exec( char *filename1, char *filename2, char *filename3){

	FILE *ptr1;
		ptr1 = fopen(filename1,"rt");
		
		FILE *ptr2;
		ptr2 = fopen(filename2,"rt");
		
		FILE *ptr3;
		ptr3 = fopen(filename3,"rt");

		if((ptr1!=NULL && ptr2!=NULL && ptr3!=NULL) && (ptr1 != ptr2) && (ptr2 != ptr3) && (ptr1 != ptr3)) {
			myInit(ptr1);
			myInit(ptr2);
			myInit(ptr3);
			scheduler();
			return 0;
		} else if((ptr1!=NULL && ptr2!=NULL && ptr3!=NULL) && (ptr1 == ptr2) && (ptr2 == ptr3)) {
			myInit(ptr1);
			scheduler();
			return 0; 
		} else if((ptr1!=NULL && ptr2!=NULL && ptr3!=NULL) && (ptr1 == ptr2) && (ptr2 != ptr3)) {
			myInit(ptr1);
			myInit(ptr3);
			scheduler();
			return 0; 
		} else if((ptr1!=NULL && ptr2!=NULL && ptr3!=NULL) && (ptr2 == ptr3) && (ptr3 != ptr1)) {
			myInit(ptr1);
			myInit(ptr2);
			scheduler();
			return 0; 
		} else if((ptr1!=NULL && ptr2!=NULL && ptr3!=NULL) && (ptr1 == ptr3) && (ptr2 != ptr3)) {
			myInit(ptr1);
			myInit(ptr2);
			scheduler();
			return 0; 
		} else if((ptr1 == NULL && ptr2!=NULL && ptr3!=NULL) && (ptr2!=ptr3)) {
			printf("Some file names may be invalid.1 \n");
			myInit(ptr2);
			myInit(ptr3);
			scheduler();
			return 0;
		} else if((ptr1 == NULL && ptr2!=NULL && ptr3!=NULL) && (ptr2==ptr3)) {
			printf("Script has alread been loaded. \n");
			myInit(ptr2);
			scheduler();
			return 0;
		} else if((ptr2 == NULL && ptr1!=NULL && ptr3!=NULL) && (ptr1!=ptr3)) {
			printf("Some file names may be invalid. 2\n");
			myInit(ptr1);
			myInit(ptr3);
			scheduler();
			return 0;
		} else if((ptr1 == NULL && ptr1!=NULL && ptr3!=NULL) && (ptr1==ptr3)) {
			printf("Script has already been loaded.1 \n");
			myInit(ptr1);
			scheduler();
			return 0;
		} else if((ptr3 == NULL && ptr1!=NULL && ptr2!=NULL) && (ptr1!=ptr2)) {
			printf("Some file names may be invalid.3 \n");
			myInit(ptr1);
			myInit(ptr2);
			scheduler();
			return 0;
		} else if((ptr1 == NULL && ptr1!=NULL && ptr2!=NULL) && (ptr1==ptr2)) {
			printf("Script has already been loaded.\n");
			myInit(ptr2);
			scheduler();
			return 0;
		} else if((ptr1==NULL) && (ptr2==NULL) && (ptr3!=NULL)) {
			printf("Only one file.\n");
			int error = run(filename3);
			return error;
		} else if((ptr1==NULL) && (ptr3==NULL) && (ptr2!=NULL)) {
			printf("Only one file.\n");
			int error = run(filename2);
			return error; 
		} else if((ptr2==NULL) && (ptr3==NULL) && (ptr1!=NULL)) {
			printf("Only one file.\n");
			int error = run(filename1);	
			return error; 
		} 

		return 0;
}

int interpreter(char buf0[], char buf1[], char buf2[], char buf3[]) {
	int result = 0; 
	if (strcmp(buf0,"help")==0) {
		printf("Legal commands:\n");
		printf("help              display this help\n");
		printf("quit              exits the shell\n");
		printf("set VAR STRING    assign STRING to VAR\n");
		printf("print VAR         display contents of VAR\n");
		printf("run SCRIPT.TXT    interpret SCRIPT.TXT\n");
		printf("exec p1 p2 p3     Executes concurrent programs\n");
		printf("                  $exec prog.txt prog2.txt\n");
		result = 0;
	}
	else if (strcmp(buf0,"quit")==0) {
		printf("Good bye.\n");
		exit(0); // exit shell code
	}
	else if (strcmp(buf0,"set")==0) {
		if (strlen(buf1)<1 || strlen(buf2)<1) return 1; // set error

		add(strdup(buf1), strdup(buf2));
	}
	else if (strcmp(buf0,"print")==0) {
		if (strlen(buf1)<1) return 2; // print error

		printf("%s\n", get(buf1));
	}
	else if (strcmp(buf0,"run")==0) {
		if (strlen(buf1)<1) return 3; // run error

		result = run(buf1);
	}
	//need to check for number of programs.
	else if (strcmp(buf0,"exec")==0) {
		if (strlen(buf1)<1) return 3; // run error
		char *words[3];
		words[0] = buf1;
		words[1] = buf2;
		words[2] = buf3;
		result = exec(buf1,buf2,buf3);
	}
	else {
		result = 98; // command does not exist
	}

	return result;
}

