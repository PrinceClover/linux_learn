#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
void clean(void *p)
{
	printf("clean:start clean thread!clean!\n");
	free(p);
}
void* thread(void* p)
{
	printf("thread:start child thread!malloc!\n");
	p = malloc(100);
	pthread_cleanup_push(clean,p);
	printf("thread:push=w=\n");
	char buf[128];
	int ret = read(0,buf,sizeof(buf));
	pthread_exit((void*)5);	
	pthread_cleanup_pop(0);
}

int main(void)
{
	pthread_t pt;
	int ret = pthread_create(&pt,NULL,thread,NULL);
	if(ret != 0)
	{
		printf("main:pthread_create error(%d)\n",ret);
		return -1;
	}
	printf("main:I will cancel child after 2s.\n");
	sleep(2);
	ret = pthread_cancel(pt);
	if(ret != 0)
	{
		printf("main:pthread_cancle error(%d)\n",ret);
		return -1;
	}
	printf("main:wait clean...\n");
	sleep(3);
	return 0;
}












