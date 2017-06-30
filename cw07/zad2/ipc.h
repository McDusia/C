#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "sys/msg.h"
#include <sys/types.h>

#define bufSize 50
char gen = 'a';
char gen2 = 'b';
char gen3 = 'c';

#define START 1
#define ECHO 2
#define MAJUSCULE 3
#define TIME 4
#define QUIT 5

#define ID 6

#define EMPTY 0
#define COMPLETE 1

#define SLEEP 0
#define WORKING 1
