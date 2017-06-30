#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //write
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

int requests = 0; //liczy prosby
int k;
int n;
int counter = 0;
pid_t pids;
pid_t *process; //processes which send request
pid_t *processesToKill; //processes which starts
int * signalList;
int * pidStatus;
sigset_t signal_set;

int position = 0;
int whoFinished = 0;
int parent;

void catchSIGUSR1(int a, siginfo_t * info, void * wsk)
{
  requests++;
  pid_t p = info->si_pid;
  printf("PARENT: receiving signal SIGUSR1\n");
  process[requests-1] = p;
   if(requests == k)
   {
     puts("I receive k requests");
     for(int j = 0; j<k;j++)
     kill(process[j],SIGUSR2);
   }
   if(requests > k)
   {
     kill(p,SIGUSR2);
   }
}

void catchSIGUSR2(int a, siginfo_t * info, void * wsk)
{
  puts("CHILD : receiving signal SIGUSR2");
}

void catchSIGCHLD(int a, siginfo_t * info, void * wsk)
{
  int b = info->si_status;
  pid_t p = info->si_pid;
  printf("receiving signal SIGCHLD from %d with status %u\n",p,b);
  for(int i = 0;i<= counter;i++)
  {
    if(processesToKill[i] == p)
      processesToKill[i] = 0;
  }
  whoFinished++;
  for(int i = 0 ;i< n;i++)
  {
    if(process[i] == p)
    {
        pidStatus[i] = b;
        break;
    }
  }
}

void handlerSIGREAL(int sig, siginfo_t *info, void *x)
{
    signalList[position] = sig;
    process[position]=(int)info->si_pid;
    printf("I receive SIGREAL from %u\n",process[position]);
    position++;
}

void end(int a)
{
  write(1, "I receive signal INT\n", 21);
  if(getpid() == parent)
  {
    for(int i = 0;i < counter;i++)
    {
      if(processesToKill[i] != 0)
        kill(processesToKill[i],SIGTERM);
    }
    for(int j=0; j<n; j++){
      if(process[j] != 0)
      printf("The PID number %d sent signal number %d. Status is %d. \n",\
      process[j], signalList[j], pidStatus[j]);
    }
  }
    exit(0);
}

int main(int argc, char *argv[])
{
  signal(SIGINT,end);
  parent = getpid();
  if(argc != 3) {
    puts("wrong number of arguments\n");
    exit(1);
  }

  n = atoi(argv[1]);
  k = atoi(argv[2]);

  struct sigaction s2;
  struct sigaction s;
  struct sigaction sigr;
  struct sigaction sU1;
  struct sigaction sCHLD;

  if(k > n){

      printf("Error. The second argument is bigger than the first one. \n");
      exit(1);
  }
  process = calloc(n,sizeof(pid_t));
  signalList = calloc(n,sizeof(int));
  pidStatus = calloc(n,sizeof(int));
  processesToKill = calloc(n,sizeof(int));

  sigfillset(&sU1.sa_mask);
  sigdelset(&sU1.sa_mask,SIGUSR1);
  sigdelset(&sU1.sa_mask,SIGINT);

  s2.sa_sigaction = catchSIGUSR1;
  sigfillset (&s2.sa_mask);
  s2.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &s2, NULL);

  s.sa_sigaction = catchSIGUSR2;
  sigfillset (&s.sa_mask);

  sigdelset(&s.sa_mask,SIGUSR2);
  sigdelset(&s.sa_mask,SIGINT);
  s.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR2, &s, NULL);

  sigr.sa_sigaction = handlerSIGREAL;
  sigemptyset(&sigr.sa_mask);
  sigaddset(&sigr.sa_mask,SIGUSR1);
  sigaddset(&sigr.sa_mask,SIGUSR2);
  sigaddset(&sigr.sa_mask,SIGCHLD);
  sigr.sa_flags=SA_SIGINFO;

  sCHLD.sa_sigaction = catchSIGCHLD;
  sigfillset (&sCHLD.sa_mask);
  sigdelset(&sCHLD.sa_mask,SIGINT);
  sCHLD.sa_flags = SA_SIGINFO;
  sigaction(SIGCHLD, &sCHLD, NULL);

  for(int i=SIGRTMIN;i<=SIGRTMAX;i++)
     sigaddset(&sigr.sa_mask,i);

  for(int i=SIGRTMIN;i<=SIGRTMAX;i++)
     sigaction(i,&sigr,NULL);

  for(int i = 0;i< n;i++)
  {
    pids=fork();
    if(pids == 0){
      //child
        processesToKill[counter] = getpid();
        printf("I am child, my pid %u\n",getpid());
        struct timespec startRealTime, endRealTime;
        srand(time(NULL));
        int sleeping = rand()%5;
        sleep(sleeping);
        clock_gettime(CLOCK_MONOTONIC_RAW, &startRealTime);
        kill(getppid(), SIGUSR1); //send request
        sigsuspend(&s.sa_mask); //wait for SIGUSR2
        int x = rand()%32;
        kill(getppid(), SIGRTMIN+x);
        printf("Moj pid: %u wyslalem SIGREAL \n",getpid());
        clock_gettime(CLOCK_MONOTONIC_RAW, &endRealTime);
        printf("The pid %d finished with time %d \n \n",\
        (int)getpid(), (int)(endRealTime.tv_sec - startRealTime.tv_sec));
        int code = (int)(endRealTime.tv_sec - startRealTime.tv_sec);
         return code;
    }
    else if(pids > 0)
    {
      sigsuspend(&sU1.sa_mask);
    }
    else
    {
      printf("Error during creating process. \n");
      return 1;
    }
    counter++;
 }
  while(whoFinished != n)
    sleep(3);

  for(int j=0; j<n; j++){
    printf("The PID number %d sent signal number %d. Status is %d. \n",\
    process[j], signalList[j], pidStatus[j]);
  }
      free(process);
      free(signalList);
      free(pidStatus);

  return 0;
}
