#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
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
struct mem_buf* addr;
int shmid;
int semNowHaircutting;
int semChairsInWaitingRoom;


void goToSleep()
{
  if(addr->barber == WORKING)
  {
    clock_gettime(CLOCK_MONOTONIC, &timer);
    printf("Sleeping... time %ld\n",(timer.tv_nsec/1000));
  }

}

void startHaircutting(pid_t pid)
{
  printf("I have just started haircutting %u\n",pid);

}

void finishHaircutting(pid_t pid)
{
  printf("I have just finished haircutting %u\n",pid);

}

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


union semun {
  int              val;
  struct semid_ds *buf;
  unsigned short  *array;
  struct seminfo  *__buf;

  };

void shiftChairs(struct mem_buf *addr, int semChairsInWaitingRoom)
{
  int emptyPlaces = semctl(semChairsInWaitingRoom, 0, GETVAL, NULL);
  int i;
  for(i = 1;i<((addr->chairsNumber)-emptyPlaces);i++)
    (addr->chairsInWaitingRoom[i-1]) = (addr->chairsInWaitingRoom[i]);

  addr->chairsInWaitingRoom[i-1]= 0;
}



int clean()
{
    int rc = shmdt(addr);
    if(rc<0)
    {
      perror("shmdt error");
      return 1;
    }
    rc = shmctl(shmid, IPC_RMID, NULL);
    if(rc<0)
    {
      perror("shmctl error");
      return 1;
    }

    rc = semctl(semNowHaircutting, 1, IPC_RMID, NULL);
    if(rc<0)
    {
      perror("semctl error");
      return 1;
    }
    rc = semctl(semChairsInWaitingRoom, 1, IPC_RMID, NULL);
    if(rc<0)
    {
      perror("semctl error");
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
  key_t semKey = ftok("/home/", gen);
  semNowHaircutting = semget(semKey, 1, 0666 | IPC_CREAT );

  //-------create 2 semaphore-----------
  key_t semKey2 = ftok("/home/", gen2);
  semChairsInWaitingRoom = semget(semKey2, 1 , 0666 | IPC_CREAT );//| IPC_EXCL);
  //int sem = semget(semKey, 0, 0666);

 //-----create shared memory
 key_t shmKey = ftok("/home/", gen3);

 shmid = shmget(shmKey, sizeof(mem) ,0666 | IPC_CREAT | IPC_EXCL);
 if(shmid <0)
 {
   perror("shmid error");
   return 1;
 }

//dołaczenie do pamięci adresowej procesu pamięci wspólnej
 addr = shmat(shmid, NULL, 0);
 if(addr == (void *)-1)
 {
   perror("shmat error");
   exit(1);
 }
 addr->nowHaircutting = 0;
 for(int i = 0;i<n;i++)
 addr->chairsInWaitingRoom[i] = 0;
 //mem.chairsInWaitingRoom = calloc(n,sizeof(pid_t)); //all EMPTY
 addr->chairsNumber = n;
 addr->barber = SLEEP;
 struct sembuf sops;
 up(semNowHaircutting,&sops);

 //-----inicialize semaphore after create-------
 union semun arg;
 arg.val = n;
 int set = semctl(semChairsInWaitingRoom, 0, SETVAL, arg);


 arg.val = 1;
 set = semctl(semNowHaircutting, 0, SETVAL, arg);
while(1)
{
  int emptyPlaces = semctl(semChairsInWaitingRoom, 0, GETVAL, NULL);

  //printf("emptyPlaces: %d",emptyPlaces);
  int saloonPlace = semctl(semNowHaircutting, 0, GETVAL, NULL);
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

    down(semNowHaircutting,&sops);

    pid_t toCut = (addr->chairsInWaitingRoom[0]);
    addr->nowHaircutting = toCut;

    shiftChairs(addr, semChairsInWaitingRoom);
    up(semChairsInWaitingRoom,&sops);

    if(toCut != 0)
    {
      clock_gettime(CLOCK_MONOTONIC, &timer);
      printf("\nNow haircutting %u time %ld\n",(addr->nowHaircutting), (timer.tv_nsec/1000));
      sleep(3);
      clock_gettime(CLOCK_MONOTONIC, &timer);
      printf("Haircutting %u finished time %ld\n",(addr->nowHaircutting), (timer.tv_nsec/1000));

      addr->nowHaircutting = 0;
    }
    //----return semaphore-----
    up(semNowHaircutting,&sops);

  }
  else{

      //start haircutting
      //printf("\nNow haircutting B %u\n",(addr->nowHaircutting));
      if(addr->nowHaircutting!=0)
      {
        sleep(3);
        clock_gettime(CLOCK_MONOTONIC, &timer);
        printf("Haircutting %u finished time %ld\n",(addr->nowHaircutting), (timer.tv_nsec/1000));
        addr->nowHaircutting = 0;
      }
  }
}
    return 0;
}
