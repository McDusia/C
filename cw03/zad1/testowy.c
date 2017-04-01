#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>


void readEnv(const char *name)
{
	puts("-----reading variables------");	
	char *val = getenv(name);
	if(val==NULL) 
		printf("Variable %s does not exist\n",name);
	else
		printf("Variable %s is %s\n", name, val);
}

int main(int argc, char *argv[])
{		

	 if(argc !=2){
		printf("Wrong number of arguments.");
		return 1;
     	}	
	
	readEnv(argv[1]);	
    	return 0;
}
