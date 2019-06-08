struct PARTITION {
    int total_blocks;
    int block_size;
} Partition;

struct FAT {
    char file_name[100];
    int file_length;
    int blockPtrs[10];
    int current_location;
} fat[20];

void initIO();
int partition(char *name, int blockSize, int totalBlocks);
int mount(char *name);
int openfile(char *name);
int readBlock(int file);
char *returnBlock();
int writeBlock(int file, char *data);
void closeFiles();
FILE *fp[5];
int partitionMounted;
int isFileOpen(char *fileName);