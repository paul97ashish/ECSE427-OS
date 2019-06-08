#include<stdlib.h>
#include<string.h>
#include<stdio.h>

// Model: Linked list
typedef struct MODEL3 {
char *var;
char *val;
struct MODEL3 *next;
} model3;

model3 *head = NULL;
//head->next = NULL;

//make inserts into one of them
void insert(char *variable, char *value){
	
	if(head==NULL){ //inserting first var
		model3 *newNode = malloc(sizeof(struct MODEL3));
	    newNode->var = strdup(variable);
		newNode->val = strdup(value);
		newNode->next = NULL;
		head = newNode;
		return;
		//break from method
	}else{
	    model3 *current = head;
	    
	    while (current->next!=NULL){
	        if (!strcmp(current->var,variable)){ //found same var
				current->val = strdup(value);
				return;
			}else{
			    current = current->next;
			}
	    }
	    
		model3 *newNode = malloc(sizeof(struct MODEL3));
	    newNode->var = strdup(variable);
		newNode->val = strdup(value);
		newNode->next = NULL;
		current->next = newNode;
	}

	
}

char *getVal(char *var) {
	model3 *current =head;
	int number=0;
	while(current!=NULL){
		if(!strcmp(current->var,var)){
			return current->val;
		}
		else{
			current = current->next;
		}
	}
	
	return "Variable does not exist";
}