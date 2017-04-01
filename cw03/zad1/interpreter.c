#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

void print(char* tab,int size){
	if(tab==NULL) puts("I receive null");	
	for(int i = 0;i<size;i++)	
	printf(" %c ",tab[i]);
	printf("\n");
}

void setEnv(const char *name, const char* val)
{
	printf("Setting variable %s\n", name);	
	setenv(name,val,0);
	char *res = getenv(name);
	if(res==NULL) 
		puts("Variable does not set \n");
	else
		printf("Variable %s set successfully to %s\n", name, val);	
}

void deleteEnv(const char *name)
{
	printf("Deleting %s\n",name);
	unsetenv(name);
}

void readEnv(const char *name)
{
	char *val = getenv(name);
	if(val==NULL) 
		printf("Variable %s does not exist\n",name);
	else
		printf("Variable %s is %s\n", name, val);
}

char* allocateSpace(size_t bufsize)
{
	char *buffer;	
	buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
		perror("Unable to allocate buffer");
		return NULL;
    }
}

char* parseFileLine(FILE * f)
{
	char *buffer;
	size_t bufsize = 32;
	size_t characters;

	if((buffer = allocateSpace(bufsize))==NULL)
		return NULL;
    if(characters = getline(&buffer,&bufsize,f)==-1)
    	return NULL;
    
	return buffer;
}

char* getName(char* buffer)
{
	int index = 0;	
	while(buffer[index]!=' ' && buffer[index]!='\n')
		index++;
	
	char* name = allocateSpace(index);
	strncpy(name,buffer+1,index-1);
	return name;
}

char* getValue(char* buffer, int start)
{
	if(buffer[start+1]=='\n')
	{
		return NULL;	
	}
	int index = start+1;	
	while(buffer[index]!='\n')
		index++;
	
	char* val = allocateSpace(strlen(buffer) - start-2);
	strncpy(val,buffer+start+2,strlen(buffer) - start-3);
	return val;
}
//strncpy(dest, src + beginIndex, endIndex - beginIndex);

  
int run(char * line)
{
	char * buffer;    
	char * command = strtok(line, " \t\n\r");
  	int n =1;
 	//tab of 10 pointers
	char * argv[10];
    	int status;
	char* data = calloc (10,sizeof(char*));
	for(int i=0; i<10; i++)
	{
		argv[i]=malloc(10*sizeof(char*));
		argv[i]=NULL;
	}

	argv[0]= command;
	while((buffer = strtok(NULL," \t\n\r")) != NULL && n<10)
	{
		argv[n]=buffer;
		n++;
	}

	 if(n==11){
	    printf("The maximum number of arguments exceeded. \n");
	    return 1;
	}

    pid_t pid=fork();
    if(pid == 0){
        if(execvp(argv[0],argv) == -1){
            printf("Error during executing command. \n");
            //free(argv);	
        return 1;
        }
    }
    else if(pid > 0)
	{
		 wait(&status);
		if(status != 0) 
		{	
			printf("Error while executing command %s \n",line);
			return 2;
		}
	    //parent - waiting
    	}
    else{
        printf("Error during creating process. \n");
        return 1;
    }
}

int main(int argc, char *argv[])
{		

	 if(argc !=2){
        printf("Too few or too many arguments.");
        return 1;
     }	
     
     FILE *f = fopen(argv[1],"r+t");
      if(f == NULL){
        perror("Error while opening file" );
        return 1;
      }
	
	char pat;
	
	char* buffer = parseFileLine(f);
	while(buffer != NULL)
	{
		if(buffer[0]=='#')
		{ 
			char* name = getName(buffer);
			char* val = getValue(buffer,strlen(name));
			if(val == NULL)
			{
				printf("\nvalue: %s to delete\n",name);
				readEnv(name);
				deleteEnv(name);
				readEnv(name);
			}
			else
			{
				printf("\nvalue:%s to set\n",name);
				readEnv(name);
				setEnv(name,val);
				readEnv(name);
			}
		}
		else
		{	if(run(buffer)==2)
				break;
		}		
		buffer = parseFileLine(f);	
	}
	
	fclose(f);	
    	return 0;
}
