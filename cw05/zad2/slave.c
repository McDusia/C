#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <complex.h>


int genRand(int max)
{
	return (rand()%max);
}

int computeIters(double re, double im, int k){
  double complex p = re + I*im;
  double complex zn = 0.0;
  int iters=0;
  while(iters<k && (cabs(zn)<2.0)){
    zn=cpow(zn,2.0) + p;
		iters++;
  }
  return iters;
}

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
      puts("Wrong number of arguments");
      return 1;
    }
    char * myfifo = argv[1];
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);

		int fd = open(myfifo, O_WRONLY);

		if(fd<0)
		{
			printf("Error while opening pipe\n");
			return 1;
		}
    srand(getpid());
    double re,im;


    for(int i = 0;i<n;i++)
    {
      re = genRand(300);
      re /= 100;
      re -= 2;
			double tmpRe = re;

      im = genRand(200);
      im /= 100;
      im -= 1;
			double tmpIm = im;
      if(re<-2 || re>1 || im < -1 || im > 1)
        printf("error - c outside of D \n");

			char tmp[50]={0x0};
			sprintf(tmp,"%f %f %d", tmpRe,tmpIm,computeIters(re,im,k));
			write(fd, tmp, sizeof(tmp));

			usleep(10000);
    }

    close(fd);
    return 0;
}
