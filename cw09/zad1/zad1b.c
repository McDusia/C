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
#include <sys/stat.h>
#include <fcntl.h>

int readersNr;
int writersNr;
int creatingFinished = 0;
int tab[100];
int descriptiveVersion = 0;

pthread_t queue[20];

pthread_t *readers;
pthread_t *writers;

int indeks = 0;

pthread_mutex_t mutexForReadersQuantity = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexForQueue = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t condForReadersQuantity = PTHREAD_COND_INITIALIZER; //na whoseTurn
pthread_cond_t condForQueue = PTHREAD_COND_INITIALIZER;  //na ilosc czytelnikow

int readersQuantity = 0;

#define READER 0
#define WRITER 1

int swapTab(pthread_t *queue, int size)
{
    printf("I've got mutex, swapTab!\n");
    pthread_t tmp = queue[0];
    for(int i = 0; i<size-1; i++)
    {
        queue[i] = queue[i+1];
    }
    queue[size-1] = -1;
    indeks--;
    printf("ten element dostanie pozwolenie: %ld\n", tmp);
    return tmp;
}

long gettid()
{
    return syscall(SYS_gettid);
}

void* readerFun(void* oneArg)
{
  int divider = *(int*)oneArg;
  pthread_t threadHandle = pthread_self();
  pthread_t threadID = gettid();
  printf("new reader with handle: %ld with tid: %ld\n", threadHandle,threadID);

  while(!creatingFinished);
  usleep(rand()%800);

  pthread_mutex_lock(&mutexForQueue);
  queue[indeks] = threadID;
  indeks++;
  while(queue[0]!=threadID)
    pthread_cond_wait(&condForQueue, &mutexForQueue);

  pthread_mutex_unlock(&mutexForQueue);

  pthread_mutex_lock(&mutexForReadersQuantity);
  readersQuantity++;
  pthread_mutex_unlock(&mutexForReadersQuantity);

  pthread_mutex_lock(&mutexForQueue);
  swapTab(queue, 20);
  pthread_cond_broadcast(&condForQueue);
  pthread_mutex_unlock(&mutexForQueue);

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

  pthread_mutex_lock(&mutexForReadersQuantity);
  readersQuantity--;
  if(readersQuantity == 0)
  pthread_cond_broadcast(&condForReadersQuantity);
  pthread_mutex_unlock(&mutexForReadersQuantity);

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
  usleep(rand()%800);

  pthread_mutex_lock(&mutexForQueue);
  queue[indeks] = threadID;
  indeks++;
  while(queue[0]!=threadID)
    pthread_cond_wait(&condForQueue, &mutexForQueue);

  pthread_mutex_unlock(&mutexForQueue);

  pthread_mutex_lock(&mutexForReadersQuantity);
  while(readersQuantity != 0)
    pthread_cond_wait(&condForReadersQuantity, &mutexForReadersQuantity);
  pthread_mutex_unlock(&mutexForReadersQuantity);

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

  printf("Writer: I have changed %d numbers\n\n", quantity);

  pthread_mutex_lock(&mutexForQueue);
  swapTab(queue, 20);
  pthread_cond_broadcast(&condForQueue);
  pthread_mutex_unlock(&mutexForQueue);

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
  pthread_mutex_destroy(&mutexForQueue);
  pthread_mutex_destroy(&mutexForReadersQuantity);
  pthread_cond_destroy(&condForQueue);
  pthread_cond_destroy(&condForReadersQuantity);

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
