#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "sys/msg.h"
#include <time.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include "ipc.h"

#include <sys/sem.h> //semafory
#include <sys/shm.h> //pamięć wspólna


struct mem_buf {
  pid_t nowHaircutting;
  pid_t chairsInWaitingRoom[100];
  int chairsNumber;
  int barber;
} mem;
struct timespec timer;

int up(int sem_id,struct sembuf *semaphore) {
  semaphore->sem_num=0;
  semaphore->sem_op=1;
  semaphore->sem_flg=0;
  int rc = semop(sem_id,semaphore,1);
  if(rc<0)
  {
    perror("semop error - ktos mnie ubiegl");
    return rc;
  }
  return 0;
}

int down(int sem_id,struct sembuf *semaphore) {
  semaphore->sem_num= 0;
  semaphore->sem_op=-1;
  semaphore->sem_flg=0;
  int rc = semop(sem_id,semaphore,1);
  if(rc<0)
  {
    perror("semop error - ktos mnie ubiegl");
    return rc;
  }
  return 0;
}


void wakeUpBarber(pid_t pid)
{
  printf("My pid %u\n",pid);
  puts("I woke up the barber");
  exit(0);
}

void waitingForBarber(pid_t pid)
{
  printf("My pid %u\n",pid);
  puts("I am waiting for the barber");
  exit(0);
}

void resignHaircutting(pid_t pid)
{
  printf("My pid %u\n",pid);
  puts("There is no empty chairs in the waiting room. I go out.");
  exit(0);
}

void finishHaircutting(pid_t pid)
{
  printf("My pid %u\n",pid);
  puts("My haircut is finished by barber");
  exit(0);
}

void printTab(pid_t *tab,int size)
{
  for(int i = 0;i<size;i++)
    printf("i=%d:%u  ",i,tab[i]);
}

int main(int argc, char *argv[])
{
  if(argc!=3)
  {
    puts("bad number of arguments");
    return 1;
  }
  int nr = atoi(argv[1]);
  int haircuts = atoi(argv[2]);

  printf("\nClients are coming...\n\n");

for(int i = 0;i<nr;i++)
{

  pid_t pid =fork();
  if(pid==0)
  {
    struct mem_buf* addr;
    int myHaircuts = 0;
    pid_t myPid = getpid();
    printf("child, my pid:%u\n",myPid);


    //-----add shared memory-------------
    key_t shmKey = ftok("/home/", gen3);
    int shmid = shmget(shmKey, 0, 0666);
    addr = shmat(shmid, NULL, 0);
    if(addr == (void *)-1)
    {
      perror("shmat error");
      return 1;
    }
    try:
    while(myHaircuts<haircuts)
    {
      struct sembuf sops;

    //--------check barber status--------

      if(addr->barber == SLEEP)
      {
        clock_gettime(CLOCK_MONOTONIC, &timer);
        printf("Barber is sleeping... let's awake him time %ld\n",(timer.tv_nsec/1000));
        //-------get 1 semaphore id-----------
        key_t semKey = ftok("/home/", gen);
        int semNowHaircutting = semget(semKey, 0, 0666);
        //------dec semaphore-----------------
        down(semNowHaircutting,&sops);

        addr->nowHaircutting = myPid;
        addr->barber = WORKING;
        myHaircuts++;

        while((addr->nowHaircutting) == myPid);
        printf("%u - my haircut nr %d finished\n", myPid, myHaircuts);
        up(semNowHaircutting,&sops);

      }
      else
        {

          sleep(2);
          //-------get 2 semaphore id-----------
          key_t semKey2 = ftok("/home/", gen2);
          int semChairsInWaitingRoom = semget(semKey2, 0, 0666);

          //------dec semaphore-----------------
          int emptyPlaces = semctl(semChairsInWaitingRoom, 0, GETVAL, NULL);
          if(emptyPlaces == 0)
          {
            clock_gettime(CLOCK_MONOTONIC, &timer);
            printf("There is no place in waiting room. I will come later time %ld\n", (timer.tv_nsec/1000));
            goto try;
          }

          down(semChairsInWaitingRoom, &sops);
          int position = (addr->chairsNumber) - emptyPlaces;
          clock_gettime(CLOCK_MONOTONIC, &timer);
          printf("I go to waitting room time %ld\n",(timer.tv_nsec/1000));

          addr->chairsInWaitingRoom[position] = myPid;
          sleep(2);
          //-------wait for cutting --------------
          while(addr->nowHaircutting != myPid);

          while(addr->nowHaircutting == myPid);
          myHaircuts++;
          printf("%u - my haircut nr %d finished\n", myPid, myHaircuts);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &timer);
    printf("I go out. All my haircuts done. Thank you. time %ld\n",(timer.tv_nsec/1000));
    return 0;
  }

  if(pid<0)
  {
    perror("fork error");
    return 1;
  }
}
  //---------wait for all children--------
  pid_t wpid;
  int status;
  wpid = wait(&status);
  while (wpid > 0)
    wpid = wait(&status);

    return 0;
}
