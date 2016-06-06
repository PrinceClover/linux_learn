#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,int *argv[])
{
	if(argc != 2)
	{
		printf("error args\n");
		return -1;
	}
	key_t key = ftok(argv[1],1);
	if(key == -1)
	{
		perror("ftok");
		return -1;
	}
	int shmid = shmget(key,20,0600|IPC_CREAT|IPC_EXCL);
	if(shmid == -1)
	{
		perror("shmget");
		return -1;
	}
	char *p;
	p = (char *)shmat(shmid,NULL,0);
	if(p == (char *)-1)
	{
		perror("shmat");
		return -1;
	}
	strcpy(p,"How are you");
	int ret = shmdt(p);
	if(ret == -1)
	{
		printf("shmctl");
		return -1;
	}
	return 0;
}
