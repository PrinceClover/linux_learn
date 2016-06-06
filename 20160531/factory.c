#include "factory.h"

void factory_init(pfact pf,int th_num,int capacity,pfunc child_fun)
{
	pf->thread = (pthread_t *)calloc(th_num,sizeof(pthread_t));
	pf->th_num = th_num;
	pthread_cond_init(&(pf->cond),NULL);
	pf->capacity = capacity;
	que_init(&(pf->que));
	pf->child_fun = child_fun;
	pf->flag = 0; 
}

void factory_start(pfact pf)
{
	int i;
	if(pf->flag == 0)
	{
		for(i = 0;i < pf->th_num;i++)
		{
			pthread_create(&(pf->thread[i]),NULL,pf->child_fun,(void*)pf);
		}
	}
	pf->flag = 1;
}

void *child_thread(void* pf)
{
	pfact p = (pfact)pf;
	int rfd;
	while(1)
	{
		pthread_mutex_lock(&p->que.mutex);
		if(p->que.size == 0)
		{
			pthread_cond_wait(&p->cond,&p->que.mutex);
		}
		que_deque(&p->que,&rfd);
		pthread_mutex_unlock(&p->que.mutex);
		send_file(rfd);
		printf("%d send successful!\n",rfd);
	}
}
