#include "head.h"
void child_handle(int fdr)
{
	int cfd;
	int flag = 1;
	while(1)
	{
		//接收客户端socket描述符
		recv_fd(fdr,&cfd);
		//向客户端发送文件
		send_file(cfd);
		//向父进程进程发送完成通知
		write(fdr,&flag,sizeof(flag));
	}
}

void make_child(pchild pc,int maxconn)
{
	int i;
	int fds[2];
	pid_t pid;
	int ret;
	for(i = 0;i < maxconn;i++)
	{
		ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		if(-1 == ret)
		{
			perror("socketpair");
			exit(-1);
		}
		pid = fork();
		if(pid == 0)//子进程
		{
			close(fds[1]);
			child_handle(fds[0]);
		}
		close(fds[0]);
		pc[i].pid = pid;
		pc[i].fds = fds[1];
		pc[i].status = FREE;
	}
}
