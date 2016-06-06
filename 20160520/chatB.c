#include"headFile.h"
//全局变量
static pid_t pid_my;
static pid_t pid_other;
static char *p;
static char buf[128]; 
static struct sembuf sp;
static struct sembuf sv;
static struct sembuf lp;
static struct sembuf lv;
static int semid;
static int shmid;
//中断响应函数SIGINT
void sendSignal(int signum,siginfo_t* info,void* p_v)
{
	printf("\nstart close ChatB!\n");
	printf("sandSignal to showChatB\n");
	//阻塞信号接收
	//sigempty(&set);
	//向showChat发送关闭消息
	semop(semid,&lp,1);
	sprintf(p,"%s","close");
	semop(semid,&sv,1);
	semop(semid,&lv,1);
}
//中断响应函数SIGILL
void closeShared(int signum,siginfo_t* info,void* p_v)
{
	printf("showChatB has been closed!\n");	
	//关闭信号量
	printf("delete sem...\n");
	int ret = semctl(semid,0,IPC_RMID);
	if(ret == -1)
	{
		perror("semctl");
		exit(-1);
	}
	printf("delete complete!\n");
	//关闭共享内存
	printf("delete shared memory...\n");
	ret = shmctl(shmid,IPC_RMID,NULL);
	if(ret == -1)
	{
		perror("shmctl");
		exit(-1);
	}
	printf("delete complete!\n");
	printf("bye bye\n");
	exit(0);
}

int main(void)
{
	int ret;
	int ret2;
	//开辟共享内存
	key_t key = ftok("chatBKey",1);
	if(key == -1)
	{
		perror("fork");
		return -1;
	}
	shmid = shmget(key,CHATBUF,0600|IPC_CREAT);
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
	//生成信号量
	semid = semget(key,2,IPC_CREAT|0600);
	if(-1 == semid)
	{
		perror("semget");
		return -1;
	}
	unsigned short array[2] = {0,1};
	ret = semctl(semid,0,SETALL,array);
	if(ret == -1)
	{
		perror("semctl_1");
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
	//开启管道
	int fdr = open("fifo_1",O_RDONLY);
	if(fdr == -1)
	{
		perror("open_read");
		return -1;
	}
	int fdw = open("fifo_2",O_WRONLY);
	if(fdw == -1)
	{
		perror("open_write");
		return -1;
	}
	printf("connected!\n");
	//发送己方pid，获取对方pid
	pid_my = getpid();
	ret = write(fdw,&pid_my,sizeof(pid_my));
	if(-1 == ret)
	{
		perror("write pid");
		return -1;	
	}
	ret = read(fdr,&pid_other,sizeof(pid_other));
	if(-1 == ret)
	{
		perror("read pid");
		return -1;
	}
//	printf("pid_other = %d\n",pid_other);
	//传递pid到共享内存
	sprintf(p,"%d",pid_my);
	//添加信号监听act1——SIGINT
	struct sigaction act1;
	act1.sa_sigaction = sendSignal;
	act1.sa_flags = SA_SIGINFO;
	ret = sigaction(SIGINT,&act1,NULL);
	if(ret == -1)
	{
		perror("sigaction:SIGINT(2)");
		return -1;
	}
	//添加信号监听act2——SIGILL
	struct sigaction act2;
	act2.sa_sigaction = closeShared;
	act2.sa_flags = SA_SIGINFO;
	ret = sigaction(SIGILL,&act2,NULL);
	if(ret == -1)
	{
		perror("sigaction:SIGILL(4)");
		return -1;
	}
	fd_set rdset;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		ret = select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret > 0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				read(0,buf,sizeof(buf));
				write(fdw,buf,strlen(buf)-1);
				semop(semid,&lp,1);
				sprintf(p,"%s%s","B:",buf);
				semop(semid,&sv,1);
				semop(semid,&lv,1);
			}
			if(FD_ISSET(fdr,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret2 = read(fdr,buf,sizeof(buf));
				if(ret2 == 0)
				{
					printf("A has exited!\n");
					break;
				}
				semop(semid,&lp,1);
				sprintf(p,"%s%s\n","A:",buf);
				semop(semid,&sv,1);
				semop(semid,&lv,1);		
			}
		}
	}
	return 0;	
}





