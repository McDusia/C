#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h> 
#include <dirent.h>
#include <time.h>

#define READ 0
#define WRITE 1
#define PATIENT 1
#define IMPATIENT 0


char wr[100];
int wrsize;
int byteNumber[100];

void print(char* tab,int size)
{
	for(int i = 0;i<size;i++)	
	printf(" %c ",tab[i]);
	printf("\n");
}

int readByte(int filedesc, int byte, char* readedByte)
{
	int size = lseek(filedesc, 0, SEEK_END);  //file size
	if(byte > size) 
	{
	  puts("File is too small");
	  return 1;
	}	
	if(lseek(filedesc,byte,SEEK_SET) < 0) 
	{
		printf("error");
	 	return 1;
	}	
	if(read(filedesc,readedByte,1) < 0)
     	{
	  write(1, "An error occurred while reading.\n", 31);
	  return 1;
	}
	printf("Readed byte: ");
	print(readedByte,1);
	return 0;	
}

int writeByte(int filedesc, int byte, char* byteToWrite)
{
	
	if(lseek(filedesc,byte,SEEK_SET) < 0) 
	{
		printf("error");
		return 1;

	}	
	if(write(filedesc, byteToWrite, 1) < 0)
     	{
	  write(1, "An error occurred while writing.\n", 31);
	  return 1;
	}
	return 0;	
}

void printInfo(struct flock* pointer)
{
	if(pointer-> l_type == F_RDLCK)
		printf("Read lock set on ");
	if(pointer-> l_type == F_WRLCK)
		printf("Write lock set on ");
	printf("%ld byte \n",pointer-> l_start);
	printf("PID process which set the lock: %d\n",pointer-> l_pid);
		
}

int checkLocks(int filedesc)
{
	struct flock* pointer = calloc(1,sizeof(struct flock));	
	puts("");	
	int size = lseek(filedesc, 0, SEEK_END);  //file size
	for(int i=0; i<size;i++)
	{	pointer-> l_whence = SEEK_SET;
		pointer-> l_type = F_WRLCK;
		pointer-> l_len = 1;		
		pointer->l_start = i;
		fcntl(filedesc, F_GETLK, pointer);
		if(pointer->l_type == F_RDLCK || pointer->l_type == F_WRLCK)
			printInfo(pointer);
	}	
	free(pointer);
	return 0;
}

void displayMyLocks(void)
{
	printf("My locks: \n");
	for(int i = 0; i< wrsize; i++)
	{
		if(wr[i]=='w')	
		{
			printf("Write lock set on %d byte\n",byteNumber[i]);
		}
		else
		{
			printf("Read lock set on %d byte\n",byteNumber[i]);	
		}
		
	}
}

int setLockOnByte(int filedesc,int byteNr, int readWrite, int user)
{
		
	struct flock* pointer = calloc(1,sizeof(struct flock));	
	
	if(readWrite==READ)
	pointer-> l_type = F_RDLCK; //lock to read
	else
	pointer-> l_type = F_WRLCK; //lock to write

	pointer-> l_whence = SEEK_SET;
	pointer-> l_start = byteNr;
	pointer-> l_len = 1;
	pointer-> l_pid = 0;
	
	//this func do not wait until the lock be released	
	if(user == IMPATIENT)	
	{	
		int er = fcntl(filedesc, F_SETLK, pointer);
		if(er==-1)
		{	
			if(readWrite==READ)
			puts("Setting the read lock failed");  
			else
			puts("Setting the write lock failed");
			//A conflicting lock is held by another process or you open the file only to write
		}	
		else
		{	if(readWrite==READ)
			{
				puts("Setting the read lock successful");
				wr[wrsize] = 'r';
				byteNumber[wrsize] = byteNr;
				wrsize++;
			}  
			else
			{
				puts("Setting the write lock successful");
				wr[wrsize] = 'w';
				byteNumber[wrsize] = byteNr;
				wrsize++;
			}
		}
	}
	else
	{	fcntl(filedesc, F_SETLKW, pointer); //waiting for opportunity
		if(readWrite==READ)
		{	
			puts("Setting the read lock successful"); 
			wr[wrsize] = 'r';
			byteNumber[wrsize] = byteNr;
			wrsize++;
		} 
		else
		{	
			puts("Setting the write lock successful");
			wr[wrsize] = 'w';
			byteNumber[wrsize] = byteNr;
			wrsize++;
		}
	}
	free(pointer);
	return 0;
}

void updateTab(int byteNr)
{
	if(byteNumber[wrsize-1]==byteNr)
	{
		wrsize--;
	}
	else
	{
		for(int i =0;i<wrsize;i++)
		{
			if(byteNumber[i]==byteNr)
			{
				byteNumber[i]=byteNumber[wrsize-1];
				wr[i] = wr[wrsize-1];
				wrsize--;
			}	
		}
	}
}

int releaseByteLock(int filedesc,int byteNr)
{		
	struct flock* pointer = calloc(1,sizeof(struct flock));
	pointer-> l_type = F_UNLCK; //release the lock
	pointer-> l_whence = SEEK_SET;
	pointer-> l_start = byteNr;
	pointer-> l_len = 1;
	pointer-> l_pid = 0;
		
	int er = fcntl(filedesc, F_SETLK, pointer);
	if(er==-1)
		puts("Release lock failed");
	else
	{
		puts("The lock is released successfully");
		updateTab(byteNr);
		
	} 
	free(pointer);
	return 0;
}

int main(int argc, char *argv[])
{
  
	if(argc < 2)
	{
		puts("Too few arguments, enter file name");
		return 1;
	}
	int byteNr,opt;
    	char tab[50];
	char pat[10];
	wrsize = 0;
	puts("------Choose one option--------");
	puts("1 - set read lock for one byte");
	puts("2 - set write lock for one byte");
	puts("3 - display locked byte list");
	puts("4 - free choose lock");
	puts("5 - read choose byte");
	puts("6 - write choose byte");
	puts("7 - exit");
	puts("------Press the button-------");	
	
	int filedesc = open(argv[1], O_RDWR);
	if(filedesc < 0)
	{
		puts("Error trying to open the file. Check if the file exists");
		return 1;
	}
	
	int fileSize = lseek(filedesc, 0, SEEK_END);
	while(1)
	{	
		fgets(tab,10,stdin);
		opt = atoi(tab);
			
		switch(opt)
		{
			case 1:
				printf("Enter byte number: ");
				fgets(tab,50,stdin);
				byteNr = atoi(tab);
				if(byteNr <= fileSize)
				{	
					puts("Are you patient? y/_");		
					fgets(pat,10,stdin);
					if(strcmp(pat,"y")==0)
						setLockOnByte(filedesc,byteNr,READ,PATIENT);
					else
						setLockOnByte(filedesc,byteNr,READ,IMPATIENT);
				}
				else
					puts("!!! The file is shorter !!!");
				break;
			case 2:
				printf("Enter byte number: ");
				fgets(tab,50,stdin);
				byteNr = atoi(tab);
				if(byteNr <= fileSize)
				{
					puts("Are you patient? y/_");		
					fgets(pat,10,stdin);
					if(strcmp(pat,"y")==0)
						setLockOnByte(filedesc,byteNr,WRITE,PATIENT);
					else
						setLockOnByte(filedesc,byteNr,WRITE,IMPATIENT);
				}
				else
					puts("!!! The file is shorter !!!");
				break;
			case 3:
				checkLocks(filedesc);
				displayMyLocks();
				break;
			case 4:
				printf("Enter byte number to unlock: ");
				fgets(tab,50,stdin);
				byteNr = atoi(tab);
				if(byteNr <= fileSize)
				{
					releaseByteLock(filedesc,byteNr);
				}
				else
					puts("!!! The file is shorter !!!");
				break;
			case 5:
				printf("Enter byte number to read: ");
				fgets(tab,50,stdin);
				byteNr = atoi(tab);
				if(byteNr <= fileSize)
				{
					char* buffor = calloc(20,sizeof(char));			
					readByte(filedesc, byteNr,buffor);
					free(buffor);
				}
				else
					puts("!!! The file is shorter !!!");
				break;
			case 6:
				printf("Enter byte number to overwrite: ");
				fgets(tab,50,stdin);
				byteNr = atoi(tab);			
				if(byteNr <= fileSize)
				{
					printf("Enter sign to write: ");
					fgets(tab,50,stdin);
					writeByte(filedesc, byteNr, tab);
				}
				else
					puts("!!! The file is shorter !!!");
				break;
			case 7:
				close(filedesc);
				return 0;
			default:
				puts("!!! That is not an option !!!");
		}
		puts("-----------------------------");
	}
	
   	return 0;
}

