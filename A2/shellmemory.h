// Memory structure type

struct shell_memory {
	char *var;
	char *value;
	struct shell_memory *next;
};

// public functions

void initShellMemory();
struct shell_memory *find(char *key);
char *get(char *var);
void add(char *var, char *value);
