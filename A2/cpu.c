#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cpu.h"
#include "kernel.h"
#include "shell.h"


void CPUrun(int quanta){
    
	for(int i = 0; i<quanta;i++){
            if(fgets(cpu->IR, sizeof(cpu->IR), cpu->IP)){
                
                int result=prompt(cpu->IR);
            }else{
               
                fileAvailable = 0;
            }
        }
}

