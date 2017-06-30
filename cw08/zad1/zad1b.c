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

int recordSize = 1024;
int threadsNr;
char *fileName;
int fd;
int numOfRecords;
int creatingFinished = 0;
char *word;
int rc;
int someonefound = 0;
pthread_t killerTID = 0;
pthread_key_t key;
pthread_t *threads;
int *wasJoin;
pthread_mutex_t mutexForRecords = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexForEnd = PTHREAD_MUTEX_INITIALIZER;
int someoneStartCancel = 0;

long gettid()
{
    return syscall(SYS_gettid);
}

void* threadFun(void *oneArg)
{
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  pthread_t threadHandle = pthread_self();
  pthread_t threadID = gettid();
  printf("new thread with handle: %ld with tid: %ld\n", threadHandle,threadID);
  char **readRecords;
  readRecords = calloc(numOfRecords, sizeof(char*));
  for(int i = 0;i<numOfRecords;i++)
    readRecords[i] = calloc(1024, sizeof(char));

  pthread_setspecific(key, readRecords);
  readRecords = pthread_getspecific(key);
  while(!creatingFinished);


  int finish = 0;
  int moreBytesToRead = 1;
  int numOfReadedBytes;

  while(!finish && moreBytesToRead)
  {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    //---------one read-------------
    for(int i=0;i<numOfRecords;i++)
    {
        numOfReadedBytes = read(fd, readRecords[i], 1024);
        if(numOfReadedBytes==-1)
        {
          perror("error while reading in threadFun");
          exit(1);
        }
        if(numOfReadedBytes<1024)
          moreBytesToRead = 0;
    }

    //------finding word in readed records----
    for(int i = 0; i<numOfRecords && !finish; i++)
    {
        if(strstr(readRecords[i], word) != NULL)
        {
          if(someoneStartCancel == 0)
          {
            someoneStartCancel = 1;
            pthread_mutex_lock(&mutexForEnd);

            killerTID = threadID;
            printf("%ld - killer\n", killerTID);
            for(int j = 0; j < threadsNr; j++)
            {
              if((wasJoin[j]!=1) && (pthread_equal(threads[j], threadHandle)==0))
                pthread_cancel(threads[j]);
            }
          }
          char recID[10];
          //----word found------
          strncpy(recID, readRecords[i], 10);
          printf("%ld: found word in record number %d\n", threadID, atoi(recID));
          someonefound = 1;
          finish = 1;
        }
    }
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
  }
  pthread_exit(NULL);
}

int openFile(char *fileName)
{
  int fd = open(fileName, O_RDONLY);
  if(fd == -1)
  {
    perror("file open error");
    exit(-1);
  }
  else
    return fd;
}

void createThread(pthread_t *thread)
{
  rc = pthread_create(thread, NULL, threadFun, NULL);
  if(rc != 0)
  {
    perror("thread create error");
    exit(-1);
  }
}

void readArgs(int argc, char *argv[])
{
  if(argc!=5)
  {
    puts("Bad number of arguments");
    puts("Appropriate arguments:");
    puts("[program] [threads nr] [file] [records nr] [word to find]");
    exit(-1);
  }
  threadsNr = atoi(argv[1]);
  fileName = calloc(1,sizeof(argv[2]));
  fileName = argv[2];
  numOfRecords = atoi(argv[3]);
  word = calloc(1,sizeof(argv[4]));
  word = argv[4];
  fd = openFile(fileName);
  return;
}

void waitForThreads()
{
  for(int i=0;i<threadsNr;i++)
  {
    int a;
    if(wasJoin[i] == 0)
    {
      if((a = pthread_join(threads[i], NULL)) !=0)
      {
        perror("error during pthread_join");
        if(a == EDEADLK)
          puts("EDEADLK");
        if(a== EINVAL)
          puts("EINVAL");
        if(a== ESRCH)
          puts("ESRCH");
      exit(-1);
      }
      else
        wasJoin[i] = 1;
    }
  }
  return;
}

void makeClean()
{
  close(fd);
  free(threads);
  pthread_key_delete(key);
  pthread_mutex_unlock(&mutexForEnd);
  pthread_mutex_destroy(&mutexForRecords);
  pthread_mutex_destroy(&mutexForEnd);
}

int main(int argc, char *argv[])
{
  readArgs(argc,argv);

  char **readRecords = calloc(numOfRecords, sizeof(char*));

  for(int i = 0;i<numOfRecords;i++)
    readRecords[i] = calloc(1024, sizeof(char));

  pthread_key_create(&key, NULL);

  threads = calloc(threadsNr, sizeof(pthread_t));
  wasJoin = calloc(threadsNr, sizeof(int));

  for(int i=0; i<threadsNr; i++)
  {
    createThread(&threads[i]);
  }
  creatingFinished = 1;

  waitForThreads();

  printf("End of program\n\n");

    if(someonefound == 0)
      puts("\tThere is no such word in your file");

  makeClean();
  return 0;
}
