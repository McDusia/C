#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h> //absolute path
#include <dirent.h>
#include <ftw.h>
#include <time.h>

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

void print(char* tab,int size){
	for(int i = 0;i<size;i++)	
	printf(" %c ",tab[i]);
	printf("\n");

}

char* formatdate(char* str, time_t val)
{
        strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}

//arg[1] - directory path
//arg[2] - [int] file size (bytes)

int printFileInfo(char* name)
{
	struct stat fileStat;
    if(lstat(name,&fileStat) < 0)    
       return 1;
 
    printf("File Size: \t\t%ld bytes\n",fileStat.st_size);
 
    printf("File Permissions: \t");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

	
    char date[36];
    printf("\nLast modification: %s\n", formatdate(date, fileStat.st_mtime));
   
	char buf[PATH_MAX + 1];
   
	char *res = realpath(name, buf);
    if (res) {
        printf("Absolute path name:  %s.\n", buf);
    } 
	return 0;
}

int isGoodSize(char* name, int size)
{
	struct stat fileStat;
    if(stat(name,&fileStat) < 0)    
       return 1;
    
	if(fileStat.st_size <= size)
	{
	  return 1; 
	}
	else
	{
		return 0; 
	}
	return 0;
}


void indwellDir(char* startPath, int size)
{
	struct dirent * file;
    DIR * path;
   
    if(( path = opendir(startPath ) ) ) 
    {
        while(( file = readdir( path ) ) )
	{
             	
		if(file->d_type == DT_REG)
		{	
		
			char res2[100000];
			strcpy(res2,startPath);
			strcat(startPath,"/");
			strcat(startPath,file->d_name);
				
			if(isGoodSize(startPath,size)==1)
			{
			  
			  	puts("\n-------INFORMATION-------\n");
				puts( file->d_name );
				//puts("-------regular file--------");
				
			  	  if(printFileInfo(startPath)==1)
					puts("problem  with printing fileInfo");	
			}
			strcpy(startPath,res2);
		}
		
		if(file->d_type == DT_DIR)		
	     	{
			if(strcmp(file->d_name,".")!=0 && strcmp(file->d_name,"..")!=0)
			{	
				char res[100000];
				strcpy(res,startPath);
			  	strcat(startPath,"/");
			  	strcat(startPath,file->d_name);
			  	indwellDir(startPath,size);
				strcpy(startPath,res);
			}
		}
	}      
        closedir( path );
    }
    else
         printf( "error - stream open error for this path, may not exist or path is empty\n");	
}

int main(int argc, char *argv[])
{
	 if(argc != 3)  
	{
		puts("Too few arguments");		
		return 1;			
	} 

	int size = atoi(argv[2]);
	indwellDir(argv[1],size);
 
    return 0;
}
