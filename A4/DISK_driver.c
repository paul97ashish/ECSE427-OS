#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "DISK_driver.h"


void initIO();
int partition(char *name, int blockSize, int totalBlocks);
int mount(char *name);
int openfile(char *name);
int readBlock(int file);
char *returnBlock();
int writeBlock(int file, char *data);
void closeFiles();

int setFile(int temp);
int isFileOpen(char *fileName);

char *block_buffer;

char fileName[80];

FILE *fp[5];
int filesFAT[5]; 

FILE *originalFile;

int partitionMounted = 0;

void initIO(){
    Partition.total_blocks = 0; 
    Partition.block_size = 0;
    
    for(int x = 0; x<20; x++){
        strcpy(fat[x].file_name, "");
        fat[x].file_length = 0;
        
        for(int i = 0; i<10; i++){
            fat[x].blockPtrs[i] = 0;
        }

        fat[x].current_location = 0;
    }

    block_buffer = NULL; 
    
    for(int x = 0; x<5; x++){
        fp[x] = NULL;
        filesFAT[x] = -1;
    } 
        
}

int partition(char *name, int blockSize, int totalBlocks){
    Partition.total_blocks = totalBlocks;
    Partition.block_size = blockSize;
 
    DIR* dir = opendir("PARTITION");

    if(dir){
    }else if(ENOENT == errno){
        system("mkdir PARTITION");
    }else{
        printf("DIRECTORY ERROR\n"); 
    }

    char createFile[80]; //create new file

    strcpy(fileName, "PARTITION/"); strcat(fileName, name); strcat(fileName, ".txt");
    strcpy(createFile, "touch "); strcat(createFile, fileName);
    system(createFile);

    printf("FILE NAME %s\n", fileName);

    FILE *openFile = fopen(fileName, "wb");
    if(openFile){
        fwrite(&Partition, sizeof(struct PARTITION), 1, openFile);
        for(int x=0; x<20;x++)
            fwrite(&fat[x], sizeof(struct FAT), 1, openFile);


        fprintf(openFile, "\n");
        char result[10];
        for(int x = 0; x<blockSize*totalBlocks; x++){
 
            fprintf(openFile, "0");  
        }
    }else{
        printf("ERROR WITH FILE\n");
    }
    fclose(openFile);
    return 1;
}

void updatePartition(){
    
    if(originalFile){
     
        fseek(originalFile, 0, SEEK_SET);
        fwrite(&Partition, sizeof(struct PARTITION), 1, originalFile);
        for(int x=0; x<20;x++)
            fwrite(&fat[x], sizeof(struct FAT), 1, originalFile);
       
    }
   
}


int mount(char *name){
    strcpy(fileName, "PARTITION/"); strcat(fileName, name); strcat(fileName, ".txt");
    originalFile = fopen(fileName, "rb+");
    if(originalFile){
        fread(&Partition, sizeof(Partition), 1, originalFile);
        for(int x=0; x<20;x++)
            fread(&fat[x], sizeof(struct FAT), 1, originalFile);

    }else{
        printf("ERROR READING FILE\n");
        return 0;
    }

    block_buffer = (char*) malloc(Partition.block_size*sizeof(char));

    printf("PARTITION %d, %d\n", Partition.block_size, Partition.total_blocks);
        
    for(int x=0; x<20; x++){
        printf("part %s\n", fat[x].file_name);
        fat[x].current_location = 0; 
    }
        
    partitionMounted = 1;
    return 1;
}
int setFile(int temp){
    FILE *openFile = fopen(fileName, "r+"); 
    FILE *fp2 = fdopen(dup(fileno(openFile)), "r+");

    int blockLocation = 0;

    for(int x = 0; x<temp; x++)
        blockLocation += fat[x].file_length;

    int ch = 0;
    while (!feof(fp2)){ 
        ch = fgetc(fp2);
        if (ch == '\n'){
            break;
        }
    }

    for (int x = 0; x < blockLocation * Partition.block_size; x++){ 
        int x = fgetc(fp2);
    }
    fclose(openFile);
    int available = 0;
    for (int x = 0; x < 5; x++){
        if (fp[x] == NULL){
            available = 1;
            fp[x] = fp2;
            filesFAT[x] = temp;
            break;
        }
    }
    if (!available){
        printf("\nUNAVAILABEL\n");
        temp = -1;
    } 

    return temp;
}
int openfile(char *name){
    int temp = -1;

    for(int x=0 ;x<20; x++){
        if(strcmp(name, fat[x].file_name) == 0){
            temp = x;
            break;
        }
    }
    printf("TEMP, %d\n", temp);
    int blockLocation = 0;


    if(temp != -1){       
        int te = setFile(temp);
        printf("Location %d Name: %s\n", blockLocation, fat[temp].file_name);
    }else{
        printf("File Doesn't exist\n");
        for(int x =0; x<20; x++){
            if(strcmp(fat[x].file_name,"")==0){
                strcpy(fat[x].file_name, name); 
                int te = setFile(x);
                updatePartition();
                return te;
            }
        }
        
    }
    printf("LAST TEMP %d\n", temp);
    updatePartition();
    return temp;
}

int readBlock(int file){
    if(file == -1 || fat[file].file_length == 0 || partitionMounted == 0){
        return -1;
    }
        
    
    int location = -1;

    for(int x = 0; x<5; x++){
        if(filesFAT[x] != -1 && filesFAT[x] == file){
            location = x;
            break;
        }
    }

    if(location == -1)
        return -1;
    
    char str[Partition.block_size];

    FILE *foundFile = fp[location];
    if(!foundFile)
        printf("BAAD STUFF\n");

    printf("About to read file: \n");
    for(int x=0; x<Partition.block_size; x++){
        if(!feof(foundFile)){
            int ch = fgetc(foundFile);
            str[x] = ch;
        }
    }
    str[Partition.block_size] = '\0';

    strcpy(block_buffer,str);
    printf("STRING: %s %lu\n", block_buffer, strlen(block_buffer));

    fat[file].current_location++; 
    return 1;
}

char *returnBlock(){
    return block_buffer;
}

int writeBlock(int file, char *data){ 
    int len = strlen(data);
    if(file == -1 || len==0 || partitionMounted == 0){
        printf("Cant read file\n");
        return -1;
    }
        

    char toWrite[100];
    strcpy(toWrite, data);
    

    int lastFat = 0;

    for(int x = 0; x<20; x++)
        if(fat[x].file_length != 0) lastFat++;
    

    printf("LAST FAT %d %d\n", lastFat, file);
    
    
    int location = -1;
    printf("fileNumber %d", file);
    for(int x = 0; x<5; x++){
        if(filesFAT[x] == file){
            location = x;
            break;
        }
    }
    if(location == -1)
        return -1;
    
    printf("Location: %d\n", location);

    FILE *openFile = fp[location];
    FILE *fp2 = fdopen(dup(fileno(openFile)), "w+");
    fseek(fp2, ftell(openFile), SEEK_SET);

    
    printf("len %d\n", len);
    
    int availableBytes = 0;

    if(lastFat == file){
    printf("File %d is last\n", file);
    for(int x=0; x<len; x++){
        int c;
        if(fp2 && !feof(fp2)){
            c = fgetc(fp2);
        } 
        else break;

        if(c == '0') availableBytes++;
        else break;
    }

    if(availableBytes == 0)
        fseek(fp2, -1, SEEK_CUR); 
    else
        fseek(fp2, -availableBytes, SEEK_CUR);

    toWrite[availableBytes] = '\0';
    len = strlen(toWrite);

    }else{
        if(len > fat[file].file_length * Partition.block_size)
            toWrite[fat[file].file_length * Partition.block_size] = '\0';
    }

    printf("Available %d\n", availableBytes);
    printf("file len %d\n", fat[file].file_length);

    printf("About to write %s to file: %s\n", data, fat[file].file_name);
    fprintf(fp2, "%s", toWrite);

    fp[location] = fp2;

    if(lastFat == file){

        if(len % Partition.block_size == 0){
            fat[file].file_length += len/Partition.block_size;
        }else{
            fat[file].file_length += (len/Partition.block_size + 1);
        }

        fat[file].current_location += fat[file].file_length;

        int totalPartiLen = 0;
        for(int x = 0; x<file;x++)
            totalPartiLen += fat[x].file_length;

        for(int x = 0; x<fat[file].file_length; x++)
            fat[file].blockPtrs[x] = totalPartiLen + x ; 
    }   
    
    updatePartition();
    return 1;
}


int isFileOpen(char *fileName){
    int file = -1;
    for(int x = 0; x<20; x++){
        if(strcmp(fileName, fat[x].file_name)==0){
            file = x;
            break;
        }
            
    }

    if(file != -1){
        for(int i = 0; i<5; i++){
            if(filesFAT[i] == file)
                return filesFAT[i];
        }
    }
    
    return -1;
}

void closeFiles(){
    for(int x = 0; x<5; x++){
        if(fp[x]) fclose(fp[x]);
    }
}