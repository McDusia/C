#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //close, usleep
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>
#include "time.h"
#include <semaphore.h>  //semaphore
#include <sys/stat.h>
#include <fcntl.h>

int readersNr;
int writersNr;
int creatingFinished = 0;
int tab[100];
int descriptiveVersion = 0;

pthread_t *readers;
pthread_t *writers;
pthread_mutex_t mutexToRead = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexToWrite = PTHREAD_MUTEX_INITIALIZER;
sem_t *semToRead;
sem_t *semToAccess;

#define READER 0
#define WRITER 1

long gettid()
{
    return syscall(SYS_gettid);
}

int up(sem_t* sem_id)
{
  int rc = sem_post(sem_id);
  if(rc<0)
  {
    return rc;
  }
  return 0;
}

int down(sem_t* sem_id)
{
//sem_wait ??
  int rc = sem_trywait(sem_id);
  if(rc<0)
  {
    return rc;
  }
  return 0;
}

void* readerFun(void* oneArg)
{
  int divider = *(int*)oneArg;
  pthread_t threadHandle = pthread_self();
  pthread_t threadID = gettid();
  printf("new reader with handle: %ld with tid: %ld\n", threadHandle,threadID);

  while(!creatingFinished);


  int tmp;

  if( sem_trywait(semToAccess) == -1 )
  {
      sem_getvalue(semToRead, &tmp);
      if(tmp == readersNr)
      {
        puts("Reader: there is writer - waiting...");
        sem_wait(semToAccess);
      }
  }

  if(sem_wait(semToRead) != 0 )
    printf("Error: waiting for reading semaphore failed.\n");
  printf("\nReader: start reading...");

  int counter = 0;
  for(int i = 0;i<100;i++)
  {
    if(tab[i]%divider == 0)
    {
      counter++;
      if(descriptiveVersion)
        printf("%d: %d\t",i,tab[i]);
    }
  }
  printf("\nReader: I have read %d numbers\n\n", counter);
  //up(semToRead);
  if(sem_post(semToRead) != 0)
    printf("Error: unlock of reading semaphore failed.\n");
    //jezeli jest ostatnia osoba ktora czyta, to oprocz sem czytania, zwalnia tez
    //sem access
  sem_getvalue(semToRead, &tmp);

  if(tmp == readersNr)
  {
      if(sem_post(semToAccess) != 0)
      printf("Error: unlock of access semaphore failed.\n");
  }

  pthread_exit(NULL);
}

int getRand(int max)
{
	return (rand()%max);
}

void* writerFun(void *oneArg)
{
  pthread_t threadHandle = pthread_self();
  pthread_t threadID = gettid();
  printf("new writer with handle: %ld with tid: %ld\n", threadHandle,threadID);

  while(!creatingFinished);

  int val;
  sem_getvalue(semToRead, &val);

  if(sem_wait(semToAccess) != 0 )
      printf("Error: waiting for writing semaphore failed.\n");
  printf("\nWriter: started writing...\n");


  int quantity = getRand(100);
  for(int i = 0;i< quantity;i++)
  {
    int position = getRand(100);
    int val = getRand(100);
    tab[position] = val;
    if(descriptiveVersion)
      printf("%d: %d\t", position, val);
  }

  //up(semToAccess);
  printf("Writer: I have changed %d numbers\n\n", quantity);
  if(sem_post(semToAccess) != 0)
    printf("Error:  incrementation of writing semaphore failed.\n");

  pthread_exit(NULL);
}

void createThread(pthread_t *thread, int who, int* divider)
{
  int rc;
  if(who == READER)
    rc = pthread_create(thread, NULL, readerFun, (void*)divider);
  else
    rc = pthread_create(thread, NULL, writerFun, NULL);

  if(rc != 0)
  {
    perror("Error: thread create");
    exit(-1);
  }
  return;
}

int readArgs(int argc, char *argv[])
{
  if(argc!=2 && argc!=3)
  {
    puts("Bad number of arguments");
    puts("Enter divider and optionally -i version");
    exit(-1);
  }
  if(argc!=2 && strcmp((argv[2]),"-i") != 0)
  {
    puts("Bad argument, available only -i option");
    exit(-1);
  }
  if(argc == 3)
    descriptiveVersion = 1;
  readersNr = 10;
  writersNr = 10;

  return atoi(argv[1]);
}

void waitForThreads()
{
  for(int i=0;i<readersNr;i++)
  {
      if(pthread_join(readers[i], NULL) !=0)
      {
        perror("error during pthread_join");
        exit(-1);
      }

  }
  for(int i=0;i<writersNr;i++)
  {
      if(pthread_join(writers[i], NULL) !=0)
      {
        perror("error during pthread_join");
        exit(-1);
      }
  }
  return;
}

void makeClean(void)
{
  puts("cleaning");
  free(readers);
  free(writers);
  int rc = sem_unlink("/toRead");
  if(rc<0)
  {
    perror("sem_unlink error");
    exit(-1);
  }

  rc = sem_close(semToRead);
  if(rc<0)
  {
    perror("sem_close error");
    exit(-1);
  }
  rc = sem_unlink("/toWrite");
  if(rc<0)
  {
    perror("sem_unlink error");
    exit(-1);
  }

  rc = sem_close(semToAccess);
  if(rc<0)
  {
    perror("sem_close error");
    exit(-1);
  }
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  int divider = readArgs(argc,argv);

  for(int i =0;i<100;i++)
    tab[i] = i;

  readers = calloc(readersNr, sizeof(pthread_t));
  writers = calloc(writersNr, sizeof(pthread_t));

  printf("readersNr: %d", readersNr);
  semToRead = sem_open("/toRead", O_CREAT, 0666, readersNr);

  int value;
  sem_getvalue(semToRead, &value);
  printf("semToRead: %d\n",value);

  int value2;
  semToAccess = sem_open("/toWrite", O_CREAT, 0666, 1);
  sem_getvalue(semToAccess, &value2);
  printf("semToAccess: %d\n",value2);

  for(int i=0; i<readersNr; i++)
    createThread(&readers[i], READER, &divider);

  for(int i=0; i<writersNr; i++)
    createThread(&writers[i], WRITER, 0);

  creatingFinished = 1;

  waitForThreads();

  printf("End of program\n\n");
  int rc = atexit(makeClean);
  if (rc != 0)
  {
     fprintf(stderr, "cannot set exit function\n");
     exit(EXIT_FAILURE);
  }

  return 0;
}
