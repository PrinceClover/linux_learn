#include"headFile.h"
//全局变量
static char* p;
static pid_t pid;
static int shmid;
static int semid;
static struct sembuf sp;
static struct sembuf sv;
static struct sembuf lp;
static struct sembuf lv;	
int main(void)
{
	//映射共享内存
	key_t key = ftok("chatAKey",1);
	if(key == -1)
	{
		perror("ftok");
		return -1;
	}
    shmid = shmget(key,CHATBUF,0600);
	if(shmid == -1)
	{
		perror("shmget");
		return -1;	
	} 
	p = (char*)shmat(shmid,NULL,0);
	if(p == (char*)-1)
	{
		perror("shmat");
		return -1;
	}
	//获取主进程pid
	pid = atoi(p);
//	printf("%s\n",p);
//	printf("pid = %d\n",pid);
	//打开信号量
	semid = semget(key,2,0600);
	if(-1 == semid)
	{
		perror("semget");
		return -1;
	}
	memset(&sp,0,sizeof(sp));
	sp.sem_num = 0;
	sp.sem_op = -1;
	sp.sem_flg = SEM_UNDO;
	memset(&sv,0,sizeof(sv));
	sv.sem_num = 0;
	sv.sem_op = 1;
	sv.sem_flg = SEM_UNDO;
	memset(&lp,0,sizeof(lp));
	lp.sem_num = 1;
	lp.sem_op = -1;
	lp.sem_flg = SEM_UNDO;
	memset(&lv,0,sizeof(lv));
	lv.sem_num = 1;
	lv.sem_op = 1;
	lv.sem_flg = SEM_UNDO;	
	while(1)
	{
		semop(semid,&sp,1);
		semop(semid,&lp,1);
		if(!strcmp(p,"close"))
		{
			int ret = shmdt(p);
			if(ret == -1)
			{
				perror("shmdt");
				return -1;
			}
			kill(pid,SIGILL);
			printf("showChatA has closed!\n");
			exit(0);
		}
		printf("%s",p);
		memset(p,0,CHATBUF);
		semop(semid,&lv,1);
	}
	return 0;
}
