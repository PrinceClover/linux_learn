#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define LIMIT 20000000

int big_int;
pthread_mutex_t pmt;

void* thread(void *p)
{
	printf("Thread:start add!\n");
	p = (int*)6;
	int i;
	for(i = 0;i < LIMIT;i++)
	{	
		pthread_mutex_lock(&pmt);
		big_int++;
		pthread_mutex_unlock(&pmt);
	}
	printf("Thread:I have added up!\n");
	pthread_exit(p);
}

int main(void)
{
	big_int = 0;
	int i;
	void *p = malloc(sizeof(int));
	pthread_t pt;
	printf("big_int = %d\n",big_int);
	int ret = pthread_mutex_init(&pmt,NULL);
	if(ret != 0)
	{
		printf("Main:pthread_mutex_init error(%d)\n",ret);
		return -1;
	}
	printf("Main:start add with child!\n");
	ret = pthread_create(&pt,NULL,thread,p);
	if(ret != 0)
	{
		printf("Main:pthread_create error(%d)\n",ret);
		return -1;
	}	
	for(i = 0;i < LIMIT;i++)
	{	
		pthread_mutex_lock(&pmt);
		big_int++;
		pthread_mutex_unlock(&pmt);
	}
	printf("Main:wait my child.\n");
	ret = pthread_join(pt,(void**)p);
	if(ret != 0)
	{
		printf("Main:pthread_join error(%d)\n",ret);
		return -1;
	}
	printf("p = %d\n",*((int*)p));
	ret = pthread_mutex_destroy(&pmt);
	if(ret != 0)
	{
		printf("Main:pthread_mutex_destory error(%d)\n",ret);
		return -1;
	}
	printf("big_int = %d\n",big_int);
	return 0;
}
