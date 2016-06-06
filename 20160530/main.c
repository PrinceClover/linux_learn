#include"head.h"

int main(int argc,char *argv[])
{
	//校验参数
	if(argc != 4)
	{
		printf("error args!\n");
		return -1;
	}
	//创建子进程
	int maxconn = atoi(argv[3]);	//最大进程数
	pchild pc = (pchild)calloc(maxconn,sizeof(child));	//子进程信息
	make_child(pc,maxconn);			//创建子进程

	/******链接准备******/
	
	//创建socket
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	//绑定ip与端口号
	int ret;
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	ret = bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		return -1;
	}
	//创建epoll描述符
	int epfd = epoll_create(1);
	struct epoll_event event;
	struct epoll_event *evs=(struct epoll_event*)calloc(maxconn+1,sizeof(event));
	//添加epoll监听
	event.events = EPOLLIN;
	event.data.fd = sfd;	
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	if(-1 == ret)
	{
		perror("epoll_ctl(sfd)");
		return -1;
	}
	int i;
	for(i = 0;i < maxconn;i++)
	{
		event.data.fd = pc[i].fds;	
		ret = epoll_ctl(epfd,EPOLL_CTL_ADD,pc[i].fds,&event);
		if(-1 == ret)
		{
			perror("epoll_ctl(pc)");
			return -1;
		}
	}
	//添加listen
	ret = listen(sfd,maxconn);	
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	printf("listening...\n");

	/*****链接并分配任务给子进程*****/
	
	int cfd;
	int flag;
	int j;
	while(1)
	{
		memset(evs,0,(maxconn+1)*sizeof(event));
		ret = epoll_wait(epfd,evs,maxconn+1,-1);
		if(ret > 0)
		{
			for(i = 0;i < ret;i++)
			{
				//接收到客户端请求
				if(evs[i].data.fd == sfd)
				{
					cfd = accept(sfd,NULL,NULL);
					if(-1 == cfd)
					{
						perror("accept");
						return -1;
					} 
					for(j = 0;j < maxconn;j++)
					{
						if(pc[j].status == FREE)
							break;
					}
					if(j > maxconn)
					{
						printf("connect failed.\n");
					}
					else
					{
						send_fd(pc[j].fds,cfd);
						pc[j].status = BUSY;
						printf("child %d is doing it\n",pc[j].pid);
						close(cfd);
					}
				}
				//查看子进程状态
				for(j = 0;j < maxconn;j++)
				{
					//子进程可读时，表示子进程工作完成
					if(evs[i].data.fd == pc[j].fds)
					{
						read(pc[j].fds,&flag,sizeof(flag));
						pc[j].status = FREE;
						printf("child %d has done the work!\n",pc[j].pid);
					}
				}
			}
		}	
	}
	return 0;
}





