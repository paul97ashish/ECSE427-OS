#include <stdio.h>
#include <string.h>
#include "pcb.h"
#include "ram.h"
#include "kernel.h"
#include "shell.h"
#include "cpu.h"
void runCPU(int quanta);

void runCPU(int quanta){
    for(int i = 0; i<quanta;i++){
            cpu->offset++;
            if(fgets(cpu->IR, sizeof(cpu->IR), cpu->IP)){
                char lastChar = cpu->IR[strlen(cpu->IR) - 1];   
                if( lastChar == '\n'){
                    cpu->IR[strlen(cpu->IR)-1] = 0;
                }

                parseInput(cpu->IR, 0);
            }else{
                fileAvailable = 0;
                return;
            }

            if(cpu->offset == 4){ 
                cpu->offset = 0;
                if(feof(cpu->IP)){ 
                    fileAvailable = 0;
                }
                kernelPageFault();
                return;
            }
        }
        temp_pcb->PC_offset = cpu->offset;
}