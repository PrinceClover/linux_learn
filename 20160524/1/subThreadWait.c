#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
typedef struct
{
	pthread_mutex_t lock;
	pthread_cond_t cond;
}cl,*pcl;

void* thread(void *p)
{
	pcl cl_t = (pcl)p;
	int ret;
	printf("Child:start,LOCK!\n");
	pthread_mutex_lock(&cl_t->lock);
	printf("Child:I will sleep.\n");
	ret = pthread_cond_wait(&cl_t->cond,&cl_t->lock);
	if(ret != 0)
	{
		printf("Child:pthread_cond_wait error(%d)\n",ret);
		return;
	}
	printf("Child:i'm wake,UNLOCK!\n");
	pthread_mutex_unlock(&cl_t->lock);
	printf("Child:byebye~~~\n");
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t pt;
	int ret;	
	cl cl1;
	printf("Main:start init mutex and cond...\n");
	ret = pthread_mutex_init(&cl1.lock,NULL);
	if(ret != 0)
	{
		printf("Main:pthread_mutex_init error(%d)\n",ret);
		return -1;
	}
	ret = pthread_cond_init(&cl1.cond,NULL);
	if(ret != 0)
	{
		printf("Main:pthread_cond_init error(%d)\n",ret);
		return -1;
	}
	printf("Main:start child...\n");
	ret = pthread_create(&pt,NULL,thread,(void*)&cl1);
	if(ret != 0)
	{
		printf("Main:pthread_create error(%d)\n",ret);
		return -1;
	}
	printf("Main:wait......\n");
	sleep(3);
	printf("Main:weak up my child\n");
	pthread_cond_signal(&cl1.cond);
	ret = pthread_join(pt,NULL);
	if(ret != 0)
	{	
		printf("Main:pthread_join error(%d)\n",ret);
		return -1;
	}
	printf("Main:destroy mutex and cond.\n");
	ret = pthread_mutex_destroy(&cl1.lock);	
	if(ret != 0)
	{	
		printf("Main:pthread_mutex_destory error(%d)\n",ret);
		return -1;
	}
	ret = pthread_cond_destroy(&cl1.cond);
	if(ret != 0)
	{	
		printf("Main:pthread_cond_destory error(%d)\n",ret);
		return -1;
	}
	printf("Main:byebye\n");
	return 0;
}
