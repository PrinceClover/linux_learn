#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
void* thread(void *p)
{
	printf("start child thread!\n");
	printf("father give me a %d.\n",*((int*)p));
	p =(void*)2;
	printf("child exit!\n");
	return p;
} 

int main()
{
	pthread_t pt;
	int *p = malloc(sizeof(int));
	*p = 1; 
	printf("p:%d\n",*p);
	int ret = pthread_create(&pt,NULL,thread,(void*)p);
	if(ret != 0)
	{
		printf("pthread_create error(%d)\n",ret);
		return -1;
	}
	printf("give child a %d.\n",*p);
	ret = pthread_join(pt,(void**)p);
	if(ret != 0)
	{
		printf("pthread_join error(%d)\n",ret);
		return -1;
	}
	printf("child give me a %d.\n",*((int *)p));
	return 0;
}
