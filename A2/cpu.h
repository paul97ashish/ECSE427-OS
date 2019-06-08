#include <stdio.h>
// do not know if this necessary yet. Revert if not required. 
struct CPU{
	FILE *IP;
	char IR[1000];
	int quanta;
};
 void CPUrun(int quanta);