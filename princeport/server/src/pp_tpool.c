/*
 *name:pp_tpool.c  // achieve function in pp_tpool
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#include "../include/pp_tpool.h"

void queInit(pQue pq)
{
	pq->phead = NULL;
	pq->ptail = NULL;
	pthread_mutex_init(&pq->mutex,NULL);
	pq->size = 0;
}

void queEnque(pQue pq,pNode pn)
{
	if(pq->size == MAX_QUE)
	{
		printf("the queue has full!\n");
		return;
	}
	pNode t_p = (pNode)calloc(1,sizeof(Node));
	t_p->rfd = pn->rfd;
	strcpy(t_p->username,pn->username);
	strcpy(t_p->path,pn->path);
	strcpy(t_p->password,pn->password);
	//lock queue
	pthread_mutex_lock(&pq->mutex);
	if(NULL == pq->phead)
	{
		pq->phead = t_p;
		pq->ptail = t_p;
	}
	else
	{
		pq->ptail->next = t_p;	//enqueue from tail
		pq->ptail = t_p;
	}
	(pq->size)++;
	//lock queue
	pthread_mutex_unlock(&pq->mutex);
}

void queDeque(pQue pq,pNode pn)
{
	pNode t_p;
	t_p = pq->phead;
	pn->rfd = t_p->rfd;
	strcpy(pn->username,t_p->username);
	strcpy(pn->path,t_p->path);
	strcpy(pn->password,t_p->password);
	pq->phead = t_p->next;
	free(t_p);
	t_p = NULL;
	if(NULL == pq->phead)
	{
		pq->ptail = 0;
	}
	(pq->size)--;
}

