#include"headFile.h"

int main(void)
{	
	//映射共享内存
	key_t key = ftok("chatBKey",1);
	if(key == -1)
	{
		perror("ftok");
		return -1;
	}
	int shmid = shmget(key,CHATBUF,0600);
	if(shmid == -1)
	{
		perror("shmget");
		return -1;	
	} 
	char *p;
	p = (char*)shmat(shmid,NULL,0);
	if(p == (char*)-1)
	{
		perror("shmat");
		return -1;
	}
	//打开信号量
	int semid = semget(key,2,0600);
	if(-1 == semid)
	{
		perror("semget");
		return -1;
	}
	struct sembuf sp;
	memset(&sp,0,sizeof(sp));
	sp.sem_num = 0;
	sp.sem_op = -1;
	sp.sem_flg = SEM_UNDO;
	struct sembuf sv;
	memset(&sv,0,sizeof(sv));
	sv.sem_num = 0;
	sv.sem_op = 1;
	sv.sem_flg = SEM_UNDO;
	struct sembuf lp;
	memset(&lp,0,sizeof(lp));
	lp.sem_num = 1;
	lp.sem_op = -1;
	lp.sem_flg = SEM_UNDO;
	struct sembuf lv;
	memset(&lv,0,sizeof(lv));
	lv.sem_num = 1;
	lv.sem_op = 1;
	lv.sem_flg = SEM_UNDO;	
	while(1)
	{
		semop(semid,&sp,1);
		semop(semid,&lp,1);
		printf("%s",p);
		memset(p,0,CHATBUF);
		semop(semid,&lv,1);
	}
	return 0;
}
