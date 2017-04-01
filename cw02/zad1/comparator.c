#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "time.h"
#include "sys/times.h"


void WriteArguments(int argc, char *argv[]) {
    printf("Arguments quantity: %d \n",argc);
    for(int i=0; i<argc; ++i)
     { 
	printf("Element number %d",i);
      	printf(" has a value:\n");
	printf(argv[i]);
	printf("\n");
     }
}

int genRand(int max)
{
	srand(time(NULL));
	return (rand()%max);
}

void print(char* tab,int size)
{
	for(int i = 0;i<size;i++)	
	printf(" %c ",tab[i]);
	printf("\n");

}
//size - byte amount to read
int readData(char* name,char* data,int size)
{
	int filedesc = open(name, O_RDWR);
	
	if(read(filedesc, data,size) < 0)
     	{
	  write(1, "An error occurred while reading.\n", 31);
	  return 1;
	}
	close(filedesc);
	return 0;	
}

int readRecord(char* name,char* record,int recSize, int index)
{
	
	int filedesc = open(name, O_RDWR);
	index = index * recSize;
	if(lseek(filedesc,index,SEEK_SET) < 0) 
	{
		printf("error");
	  	return 1;
	}	
	if(read(filedesc,record,recSize) < 0)
     	{
	  write(1, "An error occurred while reading.\n", 31);
	  return 1;
	}
	close(filedesc);
	return 0;	
}
//index - which record
int writeRecord(char* name,char* record,int recSize, int index)
{
	
	int filedesc = open(name, O_RDWR);
	index = index * recSize;
	if(lseek(filedesc,index,SEEK_SET) < 0) 
	{
		printf("error");
		return 1;

	}	
	if(write(filedesc,record,recSize) < 0)
     	{
	  write(1, "An error occurred while writing.\n", 31);
	  return 1;
	}
	close(filedesc);
	return 0;	
}

int writeData(char* name,char* data,int size)
{
	int filedesc = open(name, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR |S_IRGRP | S_IWGRP | S_IROTH);
	if(write(filedesc,data,size) != size)
        {
	
          write(1,"There was an error writing to file.txt\n",32);
          return 1;
        }
	
	close(filedesc);
	return 0;
}

void swapRecords(char* name, char* record, char* record2, int size, int i, int j){
	readRecord(name,record,size,i);
	  readRecord(name,record2,size,j);
 	  writeRecord(name,record,size,j);
	  writeRecord(name,record2,size,i);

}

void shuffle(char* name, int n, int size)
{
	for(int i = n-1;i>0;i--)
	{
	  char* record = calloc(1,size);
	  char* record2 = calloc(1,size);
	  int genRandNr = genRand(i);
	 swapRecords(name,record,record2,size,i,genRandNr);  
	}

}

void bubbleSort(char* name, int n, int size)
{
    for( int i = 0; i < n; i++ )
    {
        for( int j = 0; j < n - 1; j++ )
        {
		char* record = calloc(1,size);
	  	char* record2 = calloc(1,size);
		readRecord(name,record,size,i);
	  	readRecord(name,record2,size,j);
            if( record [0]< record2[0] )
                 swapRecords(name,record,record2,size,i,j);       
        }
    }
}

int generate(char* name, int n,int size)
{
		
	char* data = calloc (n,size);	
	int error = readData("/dev/urandom",data,n*size);
	if(error==1) return 1;
	
	int error2 = writeData(name,data,n*size);
	if(error2==1) return 1;	

	free(data);
	return 0;
}

//
int freadRecord(char* name,char* record,int recSize, int index)
{
	
	FILE *f = fopen(name,"r+t");
	index = index * recSize;
	if(fseek(f,index,SEEK_SET) != 0) 
	{
		printf("error");
	  	return 1;
	}	
	if(fread(record,recSize,1,f) != 1)
     	{
	  write(1, "An error occurred while freading.\n", 31);
	  return 1;
	}
	fclose(f);
	return 0;	
}

int freadData(char* name,char* data,int recSize, int n)
{
	FILE *f = fopen(name,"rb");

	if(fread(data,recSize,n,f) != n)
     	{ 
	write(1, "An error occurred while data freading.\n", 37);
	  return 1;
	}
	fclose(f);
	return 0;	
}

//index - which record
int fwriteRecord(char* name,char* record,int recSize, int index)
{
	
	FILE *f = fopen(name,"r+");
	index = index * recSize;
	if(fseek(f,index,SEEK_SET) < 0) 
	{
		printf("error");
	  	return 1;
	}	
	if(fwrite(record,recSize,1,f) < 0)
     	{
	  write(1, "An error occurred while freading.\n", 34);
	  return 1;
	}
	fclose(f);
	return 0;	
}

int fwriteData(char* name,char* data,int recSize, int n)
{
	FILE *f = fopen(name,"w");
	if(fwrite(data,recSize,n,f) < 0)
        {
          write(1,"There was an error writing to file.txt\n",32);
          return 1;
        }
	
	fclose(f);
	return 0;
}


void fswapRecords(char* name, char* record, char* record2, int size, int i, int j){
	freadRecord(name,record,size,i);
	freadRecord(name,record2,size,j);
 	fwriteRecord(name,record,size,j);
	fwriteRecord(name,record2,size,i);

}

void fshuffle(char* name, int n, int size)
{
	
	for(int i = n-1;i>0;i--)
	{
	  char* record = calloc(1,size);
	  char* record2 = calloc(1,size);
	  int genRandNr = genRand(i);
	 swapRecords(name,record,record2,size,i,genRandNr);  
	}

}

void fbubbleSort(char* name, int n, int size)
{
    for( int i = 0; i < n; i++ )
    {
        for( int j = 0; j < n - 1; j++ )
        {
		char* record = calloc(1,size);
	  	char* record2 = calloc(1,size);
		freadRecord(name,record,size,i);
	  	freadRecord(name,record2,size,j);
            if( record [0]< record2[0] )
                 fswapRecords(name,record,record2,size,i,j);       
        }
    }
}

int fgenerate(char* name, int n,int size)
{
		
	char* data = calloc (n,size);	
	int error = freadData("/dev/ugenRand",data,size,n);
	if(error==1) return 1;	

	
	int error2 = fwriteData(name,data,size,n);
	if(error2==1) return 1;	
	free(data);
	return 0;
}

//arg[1] - sys/lib
//arg[2] - generate/shuffle/bubble
//arg[3] - file name
//arg[4] - records quantity
//arg[5] - size of records


int main(int argc, char *argv[])
{				
	struct tms start;
	struct tms t;

	if(argc < 5)
	{	
		puts("Too few arguments");
		return 1;	
	}
	int size = atoi(argv[5]);
	int n = atoi(argv[4]);	

	if(strcmp(argv[1],"sys")==0)
	{			
		if(strcmp(argv[2],"generate")==0)
		{
		  int a = generate(argv[3],n,size);
		  if(a == 1) return 1;
		}		
		if(strcmp(argv[2],"shuffle")==0)
		{
		  times(&start);
		  shuffle(argv[3],n,size);
		  times(&t);
		  printf("START: user: %.10f, sys: %.10f\n",\
		  (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
		  (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
		
		  return 0;
		}
		else
		{
		  times(&start);
		  bubbleSort(argv[3],n,size);
		  times(&t);
		  printf("START: user: %.10f, sys: %.10f\n",\
		  (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
		  (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
		
		  return 0;
		}
	}
	if(strcmp(argv[1],"lib")==0)
	{
	 	if(strcmp(argv[2],"generate")==0)
		{
		  int a = generate(argv[3],n,size);
		  if(a == 1) return 1;
		}
		if(strcmp(argv[2],"shuffle")==0)
		{
		  times(&start);
		  fshuffle("copy.txt",n,size);
		  times(&t);
		  printf("START: user: %.10f, sys: %.10f\n",\
		  (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
		  (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
		times(&t);
		   return 0;
		}
		else
		{
		  times(&start);
		  fbubbleSort("copy.txt",n,size);
		  times(&t);
		  printf("START: user: %.10f, sys: %.10f\n",\
		  (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
		  (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
		times(&t);
		  return 0;
		}
	}
		else
		{  
		  printf("There are two options, you can use system functions or 			library functions (check first argument)\n");
		  return 0;
		}
}
