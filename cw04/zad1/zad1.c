#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //write
#include <sys/wait.h>
#include <signal.h>

char tab[35]={"ABCDEFGHIJKLMNOPQRSTUWXYZ"};
int counter = 0;
int a = 0;
void end();

struct sigaction s;

void changeDirection(int x)
{
  //write(1, "I receive signal TSTP\n", 22);
  if(a == 0)
    a = 1;
  else
    a = 0;
}

void end()
{
  write(1, "I receive signal INT\n", 21);
  exit(1);
}

int main(int argc, char *argv[])
{
  s.sa_flags = 0;
  s.sa_handler = changeDirection;
  while(1)
  {
    if(a == 0)
    {
        for(;counter<25 && a == 0;counter++)
        {

          s.sa_handler = changeDirection;
           printf("%c\n",tab[counter]);
           sleep(1);
           if(counter==24) counter=-1;
            signal(SIGINT,end);
            sigaction(SIGTSTP, &s, NULL);

        }
    }
      else{
        for(;counter>=0 && a == 1;counter--)
        {
          s.sa_handler = changeDirection;
           printf("%c\n",tab[counter]);
           sleep(1);
           if(counter==0) counter=25;
           signal(SIGINT,end);
          sigaction(SIGTSTP, &s, NULL);

        }
      }
  }
    return 0;
}
