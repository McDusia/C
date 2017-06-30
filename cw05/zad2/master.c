#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> //PIPE_BUF

#define MAX_BUF 1024

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
      puts("Wrong number of arguments");
      return 1;
    }
    char * myfifo = argv[1];
    int r = atoi(argv[2]);

    char buf[MAX_BUF];
    if(mkfifo(myfifo, 0666)==-1)
    {
      printf("Error while creating mkfifo\n");
      return 1;
    }

    int **T = calloc(r, sizeof(int*));
    for(int i=0;i<r;i++)
      T[i] = calloc(r, sizeof(int));


    for(int i=0;i<5;i++)
    {
      int pid=fork();
      if(pid==0)
      { //child
        if(execlp("./slave","./slave",myfifo,"10000","100",NULL)==-1)
        {
          puts("Error while executing command");
          return 1;
        }
        return 0;
      }
      if(pid<0)
      {
        puts("create process error");
        return 1;
      }
    }

    //int n=10;
    int filedesc = open(myfifo, O_RDONLY);
    if(filedesc<0)
    {
      printf("Error while opening pipe\n");
			return 1;
    }


      while(read(filedesc, buf, MAX_BUF)>0)
      {
        // c= x + i*y
        char *tmpX=strtok(buf, " ");
        char *tmpY=strtok(NULL, " ");
        char *tmpIters=strtok(NULL, " ");
        int iters=atoi(tmpIters);
        double x = atof(tmpX);
        double y = atof(tmpY);

        //"how many" x and y for one unit in D:
        x=(x+2.0)/3.0;
        y=(y+1.0)/2.0;

        int changedUnitX =(int)(x*(double)r);
        int changedUnitY =(int)(y*(double)r);
        T[changedUnitX][changedUnitY]=iters;
      }

    sleep(1);
    close(filedesc);
    unlink(myfifo);


    FILE *f = fopen("data","w");
    if(f == NULL){
      puts("error while opening file");
      return 1;
    }
    for(int i=0;i<r;i++)
    {
      for(int j=0;j<r;j++)
      {
        fprintf(f,"%d %d %d\n",j,i,T[j][i]);
      }
    }
    fclose(f);

    f=popen("gnuplot","w");

    fprintf(f, "set view map\n");
    fprintf(f, "set xrange[0:%d]\n", r);
    fprintf(f, "set yrange[0:%d]\n", r);
    fprintf(f, "plot 'data' with image\n");

    fflush(f);
    getc(stdin);
    pclose(f);

    return 0;
}
