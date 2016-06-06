#include "que.h"

void que_init(pQue pq)
{
	pq->phead = NULL;
	pq->ptail = NULL;
	pthread_mutex_init(&(pq->mutex),NULL);
	pq->size = 0;
}

void que_enque(pQue pq,int rfd)
{
	pNode t_p = (pNode)calloc(1,sizeof(Node));
	t_p->rfd = rfd;
	pthread_mutex_lock(&pq->mutex);
	if(pq->phead == NULL)
	{
		pq->phead = t_p;
		pq->ptail = t_p;  
	}
	else
	{
		pq->ptail->next = t_p;
		pq->ptail = t_p;
	}
	(pq->size)++;
	pthread_mutex_unlock(&pq->mutex);
}

void que_deque(pQue pq,int* rfd)
{
	pNode t_p;
	t_p = pq->phead;
	*rfd = t_p->rfd;
	pq->phead = t_p->next;
	free(t_p);
	t_p = NULL;
	if(pq->phead = NULL)
	{
		pq->ptail = NULL;
	}
	(pq->size)--;
}
