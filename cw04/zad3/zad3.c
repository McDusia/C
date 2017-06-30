#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //write
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

int k;
pid_t pids;
sigset_t signal_set;
struct sigaction s1;
struct sigaction s2;
struct sigaction s3;
struct sigaction s4;
struct sigaction sigr;
struct sigaction sCHLD;
union sigval value;
pid_t child;
pid_t parent;

int parentCounter = 0;
int childCounter = 0;

void catchSIGUSR1(int a, siginfo_t * info, void * wsk)
{
   int p = info -> si_pid;
   printf("SIG SENT PID: %u\t",p);

   if(p == child) //signal sent by  child, parent receive
    {
      puts("PARENT: I received SIGUSR1 signal");
      parentCounter++;
    }
    else{ //child receive
      puts("CHILD:  I received SIGUSR1 signal");
      childCounter++;
      kill(getppid(), SIGUSR1);
    }
}

void catchSIGUSR2(int a, siginfo_t * info, void * wsk)
{
  int p = info -> si_pid;
  if(p != child)
    {
      puts("CHILD: I received SIGUSR2 signal");
      childCounter++;
      printf("CHILD: I received %d signals\n",childCounter);
    }
}

void handlerSIGREAL(int sig, siginfo_t *info, void *x)
{
  int p = info -> si_pid;
  int signalNr = info -> si_signo;
  if(p != child)
    {
      puts("CHILD: I received SIGREAL signal");
      childCounter++;
      if(sig == (SIGRTMAX))
        kill(getppid(), SIGUSR1);
    }
}

void end(int a, siginfo_t * info, void * wsk)
{
  write(1, "\nI receive signal INT\n", 22);
  kill(child,SIGTERM);
  exit(0);
}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {  puts("wrong number of arguments\n");
    exit(1);
  }
  int l = atoi(argv[1]);
  int t = atoi(argv[2]);
  if(t != 1 && t != 2 && t != 3)
  {
    puts("Wrong value of 3rd argument");
    exit(1);
  }
  parent = getpid();
  printf("PARENT HAS PID: %u\n",parent);
  s1.sa_sigaction = catchSIGUSR1;
  sigfillset (&s1.sa_mask);
  sigdelset(&s1.sa_mask,SIGUSR1);
  s1.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &s1, NULL);


  s2.sa_sigaction = catchSIGUSR2;
  sigfillset (&s2.sa_mask);
  sigdelset(&s2.sa_mask,SIGUSR2);
  s2.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR2, &s2, NULL);


  sigr.sa_sigaction = handlerSIGREAL;
  sigemptyset(&sigr.sa_mask);
  sigaddset(&sigr.sa_mask,SIGUSR1);
  sigaddset(&sigr.sa_mask,SIGUSR2);
  sigaddset(&sigr.sa_mask,SIGCHLD);

  for(int i=SIGRTMIN;i<=SIGRTMAX;i++)
     sigaddset(&sigr.sa_mask,i);

  for(int i=SIGRTMIN;i<=SIGRTMAX;i++)
     sigaction(i,&sigr,NULL);

  sigr.sa_flags=SA_SIGINFO;
    sigaction(SIGUSR2, &sigr, NULL);

  sigfillset (&s4.sa_mask);
  sigdelset(&s4.sa_mask,SIGRTMAX);

  s3.sa_sigaction = end;
  sigfillset (&s3.sa_mask);
  sigdelset(&s3.sa_mask,SIGINT);
  s3.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &s3, NULL);

    pids=fork();

    if(pids == 0)
    {
      //child
        child = getpid();
        for(int i = 0;i<l;i++)
        {
          pause();
        }
        if(t == 1 || t == 2)
          sigsuspend(&s2.sa_mask); //waiting for SIGUSR2
        if(t == 3)
          sigsuspend(&s4.sa_mask); //waiting for SIGREAL
        return 0;
    }
    else if(pids > 0)
    {
        child = pids;

        printf("CHILD HAS PID: %u\n",child);
      for(int i = 0;i<l;i++)
      {
        if(t == 1)
          kill(child, SIGUSR1);
        if(t == 2)
          sigqueue(child, SIGUSR1, value);
          //sleep(3);
        if(t == 3)
        {
          puts("tutaj");
          kill(child, SIGRTMAX);
        }
      }
      printf("PARENT: I send %d signals\n",l);
      if(t == 1)
        kill(child, SIGUSR2);
      if(t == 2)
        sigqueue(child, SIGUSR2, value);
      if(t == 3)
      {
        srand(time(NULL));
        int x = rand()%30;
        kill(child, SIGRTMIN+x);
      }
      for(int i = 0; i< l;i++)
        sleep(1);
    }
      else
        {
          printf("Error during creating process. \n");
          return 1;
        }

  printf("PARENT: I receive %d signals.\n",parentCounter);

  while(kill(child,0) == 0)
  {
    sleep(1);
  }
  return 0;
}
