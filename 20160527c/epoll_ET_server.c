#include"head.h"

//function
//change mode of the file descriptor 
void change_fdmode(int fd)
{
	int	fdmode; 
	fdmode = fcntl(fd,F_GETFL);
	fdmode = fdmode|O_NONBLOCK;		//set fd mode to unblock
	int ret = fcntl(fd,F_SETFL,fdmode);
	if(-1 == ret)
	{
		perror("fcntl");
		return;
	}
}

int main(int argc,char* argv[])
{	
	//variates definition
	int sfd,cfd;			//socket discriptor of server/client
	int epfd;						//epoll discriptor
	int ret,ret2;						//receive return value
	char buf[BUFLEN];				//buffer
	struct sockaddr_in server;		//connection information of server
	struct sockaddr_in client;		//connection informaiton of client
	int addrlen = sizeof(struct sockaddr);//length of struct sockaddr 
	struct epoll_event event,cev[1+MAXCONN];//epoll listening event(s)
	int i;						//loop variates	

	//check args
	if(argc != 3)
	{
		printf("error args!\n");
		return -1;
	}
	//create socket
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	//create epoll
	epfd = epoll_create(1);
	if(-1 == epfd)
	{
		perror("epoll_create");
		return -1;
	}
	
	//bind ip address and port
	memset(&server,0,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	ret = bind(sfd,(struct sockaddr*)&server,sizeof(server));
	if(-1 == ret)
	{
		perror("bind");
		return -1;
	}
	
	//listen client connecting
	ret = listen(sfd,MAXCONN);
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	printf("listening...\n");

	//register sfd/stdin into epoll
	event.events = EPOLLIN;			
	event.data.fd = sfd; 
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	if(-1 == ret)
	{
		perror("epoll_ctl(sfd)");
		return -1;		
	}
	event.events = EPOLLIN;
	event.data.fd = 0;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
	if(-1 == ret)
	{
		perror("epoll_ctl(stdin)");
		return -1;		
	}
	//start data change
	while(1)
	{
		memset(cev,0,sizeof(cev));
		ret = epoll_wait(epfd,cev,1+MAXCONN,-1);
		if(ret > 0)
		{
			for(i = 0;i < ret;i++)
			{
				//sfd do
				if(cev[i].events == EPOLLIN && cev[i].data.fd == sfd)
				{
					//accept connection of client	
					memset(&client,0,sizeof(client));
					cfd = accept(sfd,(struct sockaddr*)&client,&addrlen);
					if(-1 == cfd)
					{
						perror("accept");
						return -1;						
					}
					printf("ip:%s  PORT:%d is connecting\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
					event.events = EPOLLIN|EPOLLET;	//ET mode
					change_fdmode(cfd);				//change fd to unblock
					event.data.fd = cfd;
					epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&event);
				}
				//stdin do
				if(cev[i].events == EPOLLIN && cev[i].data.fd == 0)
				{
					memset(buf,0,sizeof(buf));
					ret2 = read(0,buf,sizeof(buf));
					if(ret2 > 0)
					{
						send(cfd,buf,strlen(buf)-1,0);
					}
					else if(0 == ret2)
					{
						printf("bye bye %d\n",cfd);
						event.events = EPOLLIN;
						event.data.fd = cfd;
						epoll_ctl(epfd,EPOLL_CTL_DEL,cfd,&event);
						close(cfd);
					}
				}
				//cfd do
				if(cev[i].events == EPOLLIN && cev[i].data.fd == cfd)
				{
					while(1)
					{
						memset(buf,0,sizeof(buf));
						ret2 = recv(cfd,buf,sizeof(buf),0);
						if(ret2 > 0)
						{
							printf("%s",buf);
						}
						else if(ret2 == -1 && errno == EAGAIN)
						{
							break;
						}
						else if(ret2 == 0)
						{
							printf("clinet %d has close the connection!\n",cfd);
							event.events = EPOLLIN;
							event.data.fd = cfd;
							epoll_ctl(epfd,EPOLL_CTL_DEL,cfd,&event);
							close(cfd);
						}
					}
					printf("\n");
				}
			}
		}
	}
	return 0;
}


