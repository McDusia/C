#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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

struct mem_buf* addr;
int shmid;
sem_t *semNowHaircutting;
sem_t *semChairsInWaitingRoom;


void goToSleep()
{
  if(addr->barber == WORKING)
  puts("Sleeping...");

}

void startHaircutting(pid_t pid)
{
  printf("I have just started haircutting %u\n",pid);

}

void finishHaircutting(pid_t pid)
{
  printf("I have just finished haircutting %u\n",pid);

}

int up(sem_t* sem_id) {

  int rc = sem_post(sem_id);
  if(rc<0)
  {
    perror("sem_post - ktos mnie ubiegl");
    return rc;
  }
  return 0;
}


int down(sem_t* sem_id) {

  int rc = sem_wait(sem_id);
  if(rc<0)
  {
    perror("semop error - ktos mnie ubiegl");
    return rc;
  }
  return 0;
}

void shiftChairs(struct mem_buf *addr, sem_t* semChairsInWaitingRoom)
{
  int emptyPlaces;
  sem_getvalue(semChairsInWaitingRoom, &emptyPlaces);
  int i;
  for(i = 1;i<((addr->chairsNumber)-emptyPlaces);i++)
    (addr->chairsInWaitingRoom[i-1]) = (addr->chairsInWaitingRoom[i]);

  addr->chairsInWaitingRoom[i-1]= 0;
}


int clean()
{
    //----clean shared memory------
    int rc = shm_unlink("/third");
    if(rc<0)
    {
      perror("munmap error");
      return 1;
    }

    rc = munmap(addr,sizeof(mem));
    if(rc<0)
    {
      perror("munmap1 error");
      return 1;
    }

    //----clean semaphores---------
    rc = sem_unlink("/first");
    if(rc<0)
    {
      perror("sem_unlink error");
      return 1;
    }
    rc = sem_unlink("/second");
    if(rc<0)
    {
      perror("sem_unlink error");
      return 1;
    }

    rc = sem_close(semNowHaircutting);
    if(rc<0)
    {
      perror("sem_close error");
      return 1;
    }
    rc = sem_close(semChairsInWaitingRoom);
    if(rc<0)
    {
      perror("sem_close error");
      return 1;
    }


    return 0;
}

void end()
{
  write(1, "I receive signal INT\n", 21);
if(clean() == 0)
  exit(0);
  else exit(1);
}



int main(int argc, char *argv[])
{
  if(argc!=2)
  {
    puts("bad number of arguments");
    return 1;
  }
  signal(SIGINT,end);
  int n = atoi(argv[1]);
  mem.nowHaircutting = 0;
  for(int i = 0;i<n;i++)
  mem.chairsInWaitingRoom[i] = 0;
  mem.chairsNumber = n;
  mem.barber = SLEEP;

  int rc;
  printf("\nBarber invites! Waiting for clients...\n\n");

  //-------create 1 semaphore-----------
  semNowHaircutting = sem_open("/first", O_CREAT | O_EXCL, 0666, 0);

  //-------create 2 semaphore-----------
  semChairsInWaitingRoom = sem_open("/second", O_CREAT | O_EXCL, 0666, n);

 //-----create shared memory
 //shm_id=shm_open("barberqueue",O_CREAT | O_EXCL | O_RDWR,POSIXMODE)    ;
 //gdzie:
 //#define POSIXMODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
 shmid = shm_open("/third", O_CREAT | O_RDWR, 0666 );
 if(shmid <0)
 {
   perror("shm_open error");
   return 1;
 }
 //------set size of shared memory segment------

 rc = ftruncate(shmid,(int)sizeof(mem));
 if(rc<0)
 {
   perror("ftruncate error");
   return 1;
 }

//dołaczenie do pamięci adresowej procesu pamięci wspólnej
 addr = mmap(NULL, (int)sizeof(mem), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shmid, 0);
 if(addr == (void *)-1)
 {
   perror("mmap error");
   exit(1);
 }

 addr->nowHaircutting = 0;
 for(int i = 0;i<n;i++)
 addr->chairsInWaitingRoom[i] = 0;
 //mem.chairsInWaitingRoom = calloc(n,sizeof(pid_t)); //all EMPTY
 addr->chairsNumber = n;
 addr->barber = SLEEP;

 up(semNowHaircutting);


while(1)
{
  int emptyPlaces;
  sem_getvalue(semChairsInWaitingRoom, &emptyPlaces);

  //printf("emptyPlaces: %d",emptyPlaces);
  int saloonPlace;
  sem_getvalue(semNowHaircutting, &saloonPlace);
  //printf("saloonPlace:%d",saloonPlace);

  if((emptyPlaces == n) && (saloonPlace ==1))
  {

    goToSleep();
    addr->barber = SLEEP;
  }
  else if((emptyPlaces < n) && (saloonPlace ==1))
  {
    puts("Oh... some clients");
    addr->barber = WORKING;

    down(semNowHaircutting);

    pid_t toCut = (addr->chairsInWaitingRoom[0]);
    addr->nowHaircutting = toCut;

    shiftChairs(addr, semChairsInWaitingRoom);
    up(semChairsInWaitingRoom);

    if(toCut != 0)
    {
      printf("\nNow haircutting A %u\n",(addr->nowHaircutting));
      sleep(3);
      printf("Haircutting %u finished\n",(addr->nowHaircutting));
      addr->nowHaircutting = 0;
    }
    //----return semaphore-----
    up(semNowHaircutting);

  }
  else{

      //start haircutting
      //printf("\nNow haircutting B %u\n",(addr->nowHaircutting));
      if(addr->nowHaircutting!=0)
      {
        sleep(3);
        printf("Haircutting %u finished\n",(addr->nowHaircutting));
        addr->nowHaircutting = 0;
      }
  }
}
    return 0;
}
