#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

char* programs[20];
char* arguments[3];

int parse()
{
	size_t bufsize = 50;
  char *buffer = calloc(bufsize,sizeof(char));
	char *token;
  size_t characters;

   printf("Type line: ");
   if((characters = getline(&buffer,&bufsize,stdin))==-1)
   {
     printf("There is no any characters in stdin\n");
     exit(0);
   }

	 token = strtok(buffer, "|");
	 programs[0] = token;
	 int i = 1;
	 while( token != NULL )
   {
      token = strtok(NULL, "|");
			if(token != NULL)
			{
				programs[i] = token;
				i++;
			}
   }
	 int j=0;
	 while((programs[i][j]) != '\n')
	 j++;
	 programs[i][j]=NULL;

	 return i;
}

int getArguments(char* buffer, int size)
{
	 char *token = strtok(buffer," ");
	 arguments[0] = token;
	 int i = 1;
	 while( token != NULL)
   {
      token = strtok(NULL," ");
			if(token!=NULL)
			{
				arguments[i] = token;
				i++;
			}
   }
	 return i;
}

int main(int argc, char *argv[])
{
	while(1)
	{
		 for(int i= 0;i<20;i++)
			programs[i] = calloc(20, sizeof(char));
		 for(int i= 0;i<3;i++)
			arguments[i] = calloc(20, sizeof(char));

	   int progNr = parse();
		 int fd[19][2];

		 for(int i = 0; i< (progNr-1);i++)
		 	pipe(fd[i]);

		 for(int i = 0;i<progNr;i++)
		 {
			 printf("i: %d\n",i);

			 pid_t pid = fork();
			 if (pid == 0)
			 {
				 printf("I am a child, %u \n",getpid());
				 printf("TEST: %s \n", programs[i]);
				 int c = getArguments(programs[i],sizeof(programs[i]));
				 for(int i=0;i<c;i++)
				 		printf("Argument %d: %s\n",i,arguments[i]);
				 for(int i=c;i<3;i++)
						arguments[i] = NULL;

					if(i==0)
					{
						close(fd[0][0]);
						dup2(fd[0][1],STDOUT_FILENO);
						close(fd[0][1]);
						execvp(arguments[0],arguments);
					}
					else if(i==progNr-1)
					{
						close(fd[i-1][1]);
						dup2(fd[i-1][0],STDIN_FILENO);
						close(fd[i-1][0]);
						execvp(arguments[0],arguments);
					}
					else
						{
								close(fd[i-1][1]);
								dup2(fd[i-1][0],STDIN_FILENO);
								close(fd[i-1][0]);

								close(fd[i][0]);
								dup2(fd[i][1],STDOUT_FILENO);
								close(fd[i][1]);
								execvp(arguments[0],arguments);
						}
				 exit(0);
			 }
			 else
			 {

				puts("parent");
				printf("czekam na %u \n", pid);
				 if(i != 0)
				 close(fd[i-1][1]);
				 close(fd[i-1][0]);
				}
			}

			wait(NULL);
			printf("\n");
			sleep(2);
	}
    	return 0;
}
