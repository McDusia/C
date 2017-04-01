#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  //pid_t, rlim_t (unsigned int)
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

void print(char* tab,int size){
	if(tab==NULL) puts("I receive null");	
	for(int i = 0;i<size;i++)	
	printf(" %c ",tab[i]);
	printf("\n");
}

void setEnv(const char *name, const char* val)
{
	//printf("Setting variable %s\n", name);	
	setenv(name,val,0);
	char *res = getenv(name);
	if(res==NULL) 
		puts("Variable does not set \n");
	else;
		//printf("Variable %s set successfully to %s\n", name, val);	
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
		printf("Variable %s is %s", name, val);
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

  
int run(char * line, char* newTimeLimit, char* newMemLimit)
{	


	int newTimeLimitInt =atoi(newTimeLimit);
	int newMemLimitInt =atoi(newMemLimit);
	if(newTimeLimitInt <= 0 || newMemLimitInt <= 0){
		puts("\nError - time limit or mem limit is negative or 0 or the number is to big to read.");
	       	return 1;
	}
	unsigned long int t = atoi(newTimeLimit);
	unsigned long int m = atoi(newMemLimit);
		printf("\nTime limit: %lu s.\n",t);
		m *= 1024*1024;
		printf("Memory limit: %lu bytes. \n",m);
	
	unsigned int maxVal = 4294967295;
	if(t > maxVal || m > maxVal)
	{	
		puts("Your limits are to high to read it. This number is to big for unsigned int.");
		return 1;	
	}

	char * buffer;    
	char * command = strtok(line, " \n\t\r");
	struct rusage r;
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
	struct rlimit oldMemLimit;
	struct rlimit oldTimeLimit;
	
 	if(getrlimit(RLIMIT_AS, &oldMemLimit) == -1){
        puts("Problem with getting virtual memory limit.");
        return 1;
    	}


	if(getrlimit(RLIMIT_CPU, &oldTimeLimit) == -1){
		puts("Problem with getting CPU time limit.");
		return 1;
	}
	
	struct rlimit setTimeLimit;
	struct rlimit setMemLimit; 
	setTimeLimit.rlim_max=atoi(newTimeLimit);  //hard limit
	setTimeLimit.rlim_cur=atoi(newTimeLimit)/2; //soft limit
	setMemLimit.rlim_max=atoi(newMemLimit)*1024*1024;
	setMemLimit.rlim_cur=(atoi(newMemLimit)*1024*1024)/2;

	pid_t pid=fork(); //new child process
	if(pid == 0)
	{
	//setting memory limit 
		if(setrlimit(RLIMIT_AS, &setMemLimit) == -1)
		{
			puts("Setting memory limit failed. \
			Current usage is already higher than the limit");
			return 1;
		}
	//setting time limit
		if(setrlimit(RLIMIT_CPU, &setTimeLimit) == -1)
		{
			puts("Setting time limit failed.");
			return 1;
		}

		if(execvp(argv[0],argv) == -1)
		{
			puts("Error during execvp.");
			return 1;
		}
	}
    	else if (pid > 0 )
	    {	wait3(&status,0,&r);
			double userTime = r.ru_utime.tv_sec;
			printf(" User time: %f \n", userTime);
			double sysTime = r.ru_stime.tv_sec;
			printf(" System time: %f \n", sysTime);
			if(status != 0)
			{
				printf("Error while executing command %s \n",line);
				return 1;
			}

		}
			else
			{
				printf("Error during creating process. \n");
				return 1;
			}
}

int main(int argc, char *argv[])
{		

	 if(argc !=4){
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
				deleteEnv(val);
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
		else{
			int ex = run(buffer,argv[2],argv[3]);
			if(ex == 1)
				return 1;
		}
		buffer = parseFileLine(f);	
	}
	
	fclose(f);	
    	return 0;
}
