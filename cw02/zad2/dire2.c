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
#include <limits.h> //absolute path
#include <dirent.h>
#include <time.h>

int size;

char* formatdate(char* str, time_t val)
{
        strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    if(tflag == FTW_F && sb->st_size<=size)
    {
	    puts("\n-------INFORMATION-------\n");
	    printf("%s\n",fpath + ftwbuf->base);
    	    printf("---------------------------\n");
    	    printf("File Size: \t\t%ld bytes\n",sb->st_size);	    
	    printf("File Permissions: \t");
	    printf( (S_ISDIR(sb->st_mode)) ? "d" : "-");
	    printf( (sb->st_mode & S_IRUSR) ? "r" : "-");
	    printf( (sb->st_mode & S_IWUSR) ? "w" : "-");
	    printf( (sb->st_mode & S_IXUSR) ? "x" : "-");
	    printf( (sb->st_mode & S_IRGRP) ? "r" : "-");
	    printf( (sb->st_mode & S_IWGRP) ? "w" : "-");
	    printf( (sb->st_mode & S_IXGRP) ? "x" : "-");
	    printf( (sb->st_mode & S_IROTH) ? "r" : "-");
	    printf( (sb->st_mode & S_IWOTH) ? "w" : "-");
	    printf( (sb->st_mode & S_IXOTH) ? "x" : "-");
	    printf("\n");

		char date[36];
	    printf("Last modification: %s\n", formatdate(date, sb->st_mtime));
	   
		char buf[PATH_MAX + 1];
	   
		char *res = realpath(fpath, buf);
	    if (res) {
		printf("Absolute path name:  %s.\n", buf);
	    } 
     }
     return 0;           /* To tell nftw() to continue */
}

int main(int argc, char *argv[])
{
    if(argc != 3)  
{
	puts("Too few arguments");		
	return 1;			
} 
   
   int flags =  FTW_PHYS | FTW_DEPTH;;
   size = atoi(argv[2]);
   if (nftw((argc < 2) ? "." : argv[1], display_info, 20, flags)
           == -1) {
       perror("nftw");
       exit(EXIT_FAILURE);
   }
   exit(EXIT_SUCCESS);
}

