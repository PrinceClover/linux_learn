#include"headFile.h"

int main(void)
{
	int ret;
	int ret2;
	char buf[128];
	//开辟共享内存
	key_t key = ftok("chatAKey",1);
	if(key == -1)
	{
		perror("fork");
		return -1;
	}
	char *p;
	int shmid = shmget(key,CHATBUF,0600|IPC_CREAT);
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
	int semid = semget(key,2,IPC_CREAT|0600);
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
/*	memset(array,0,sizeof(array));
	ret = semctl(semid,0,GETALL,array);
	if(-1 == ret)
	{
		perror("semctl_2");
		return -1;
	}
	printf("array[0] = %d, array[1] = %d\n",array[0],array[1]);
*/	struct sembuf sp;
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
	//开启管道
	int fdw = open("fifo_1",O_WRONLY);
	if(fdw == -1)
	{
		perror("open_write");
		return -1;
	}
	int fdr = open("fifo_2",O_RDONLY);
	if(fdr == -1)
	{
		perror("open_read");
		return -1;
	}
	printf("connected!\n");
	
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
				sprintf(p,"%s%s","A:",buf);
				semop(semid,&sv,1);
				semop(semid,&lv,1);
			}
			if(FD_ISSET(fdr,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret2 = read(fdr,buf,sizeof(buf));
				if(ret2 == 0)
				{
					printf("B has exited!\n");
					break;
				}
				semop(semid,&lp,1);
				sprintf(p,"%s%s\n","B:",buf);
				semop(semid,&sv,1);
				semop(semid,&lv,1);		
			}
		}
	}
	return 0;	
}





