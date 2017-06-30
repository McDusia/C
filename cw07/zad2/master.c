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

//POSIX
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

struct mem_buf {
  pid_t nowHaircutting;
  pid_t chairsInWaitingRoom[100];
  int chairsNumber;
  int barber;
} mem;

struct timespec timer;

int up(sem_t *sem_id) {

  int rc = sem_post(sem_id);
  if(rc<0)
  {
    perror("sem_post - ktos mnie ubiegl");
    return rc;
  }
  return 0;
}


int down(sem_t *sem_id) {

  int rc = sem_wait(sem_id);
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
}

void waitingForBarber(pid_t pid)
{
  printf("My pid %u\n",pid);
  puts("I am waiting for the barber");
}

void resignHaircutting(pid_t pid)
{
  printf("My pid %u\n",pid);
  puts("There is no empty chairs in the waiting room. I go out.");
}

void finishHaircutting(pid_t pid)
{
  printf("My pid %u\n",pid);
  puts("My haircut is finished by barber");
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
    int shmid = shm_open("/third", O_RDWR, 0666);
    if(shmid <0)
    {
      perror("shm_open error");
      return 1;
    }
    /*int rc = ftruncate(shmid,sizeof(mem)); //????moze nie trzeba
    if(rc<0)
    {
      perror("ftruncate error");
      return 1;
    }*/
    addr = mmap(NULL,sizeof(mem), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shmid, 0);
    if(addr == (void *)-1)
    {
      perror("mmat error");
      return 1;
    }
    try:
    while(myHaircuts<haircuts)
    {

    //--------check barber status--------

      if(addr->barber == SLEEP)
      {
        puts("Barber is sleeping... let's awake him");
        //-------get 1 semaphore id-----------

        sem_t *semNowHaircutting = sem_open("/first", O_RDWR, 0666, 1);
        //------dec semaphore-----------------
        down(semNowHaircutting);

        addr->nowHaircutting = myPid;
        addr->barber = WORKING;
        myHaircuts++;

        while((addr->nowHaircutting) == myPid);
        printf("%u - my haircut nr %d finished\n", myPid, myHaircuts);
        up(semNowHaircutting);

      }
      else
        {

          sleep(2);
          //-------get 2 semaphore id-----------

          sem_t *semChairsInWaitingRoom = sem_open("/second", O_CREAT, 0666, 4);


          int emptyPlaces;
          sem_getvalue(semChairsInWaitingRoom, &emptyPlaces);


          if(emptyPlaces == 0)
          {
            clock_gettime(CLOCK_MONOTONIC, &timer);
            printf("There is no place in waiting room, time: %ld. I will come later",(timer.tv_nsec/1000));
            goto try;
          }

          down(semChairsInWaitingRoom);
          int position = (addr->chairsNumber) - emptyPlaces;


          addr->chairsInWaitingRoom[position] = myPid;
          clock_gettime(CLOCK_MONOTONIC, &timer);
          printf("%d go to waitting room, time: %ld\n",myPid,(timer.tv_nsec/1000));
          sleep(2);
          //-------wait for cutting --------------
          while(addr->nowHaircutting != myPid);

          while(addr->nowHaircutting == myPid);
          myHaircuts++;

          printf("%u - my haircut nr %d finished \n", myPid, myHaircuts);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &timer);
    printf("I go out. All my haircuts done. Thank you. time: %ld\n",(timer.tv_nsec/1000));
    
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
