#include"head.h"

int main(int argc,char* argv[])
{
	//variates definition
	int cfd;						//socket discriptor of clinet
	int ret,ret2;					//receive return value
	int epfd;						//epoll discriptor of clinet
	char buf[128];					//buffer
	struct sockaddr_in client;		//connection information of client
	struct epoll_event event,cev[2];//epoll listening event
	int i;							//loop variate
	
	//check args
	if(argc != 3)
	{
		printf("error args!\n");
		return -1;
	}

	//create socket
	cfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == cfd)
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
	
	//connceting server
	memset(&client,0,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(argv[1]);
	client.sin_port = htons(atoi(argv[2]));
	ret = connect(cfd,(struct sockaddr*)&client,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("connect");
		return -1;
	}
	printf("ip:%s  PORT:%d is connecting!\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

	//register cfd/stdin into epoll
	event.events = EPOLLIN;
	event.data.fd = cfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&event);
	if(-1 == ret)
	{
		perror("epoll_ctl(cfd)");
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
		ret = epoll_wait(epfd,cev,2,-1);
		if(ret > 0)
		{
			for(i = 0;i < ret;i++)
			{
				//stdin do
				if(cev[i].events == EPOLLIN && cev[i].data.fd == 0)
				{
					memset(buf,0,sizeof(buf));
					ret2 = read(0,buf,sizeof(buf));
					if(ret2 == 0)
					{
						printf("bye bye\n");
						close(cfd);
						return 0;
					}
					ret2 = send(cfd,buf,strlen(buf)-1,0);
					if(-1 == ret2)
					{
						perror("send");
						return -1;
					}
				}
				//cfd do
				if(cev[i].events == EPOLLIN && cev[i].data.fd == cfd)
				{
					memset(buf,0,sizeof(buf));
					ret2 = recv(cfd,buf,sizeof(buf),0);
					if(ret2 > 0)
					{
						printf("%s\n",buf);
					}
					else if(ret2 == 0)
					{
						printf("server has close the connection!\n");
						close(cfd);
						return 0;;
					}
				}
		
			}
		}
	}
	return 0;
}
