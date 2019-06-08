#include<stdlib.h>
#include<string.h>
#include "shellmemory.h"

// data structure for the shell memory

struct shell_memory *memory, *memoryTail;

// public functions

void initShellMemory() {
	memory     = NULL;
	memoryTail = NULL;
}

struct shell_memory* find(char *key) {
	struct shell_memory *temp = memory;

	while(temp!=NULL) {
		if (strcmp(key,temp->var)==0) return temp;
		temp = temp->next;
	}

	return NULL;
}

void add(char *var, char *value) {
	struct shell_memory *temp;

	// does the var already exist? if so replace, otherwise add new
	
	temp = find(var);

	if (temp != NULL) { // overwrite
		free(temp->value);
		temp->value = strdup(value);
	}
	else {
		temp = (struct shell_memory *) malloc(sizeof(struct shell_memory));
		temp->var   = strdup(var);
		temp->value = strdup(value);
		temp->next  = NULL;

		if (memory     == NULL) memory     = temp;
		if (memoryTail == NULL) memoryTail = temp;
		else {
			memoryTail->next = temp;
			memoryTail = temp;
		}
	}
}

char *get(char *var) {
	struct shell_memory *temp;

	temp = find(var);

	if (temp != NULL)
		return temp->value;
	else
		return "not found";
}

