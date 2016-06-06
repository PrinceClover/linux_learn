#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define CHATBUF 2048
