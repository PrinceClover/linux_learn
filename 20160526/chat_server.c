#include"head.h"

int main(int argc,char** argv)
{
	//variate definition
	int ret;					//receive return value
	int sfd,cfd;				//server/client socket discriptors	
	struct sockaddr_in server; 	//connection information of server
	struct sockaddr_in client;	//connection information of client	
	char buf[BUFLEN];			//sand/receive buffer
	fd_set rdset;				//read set
	int addrlen;				//connection information length
	
	//chack args
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}

	//create socket
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}

	//band port
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
	
	//add listen
	ret = listen(sfd,MAXCONN);
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	printf("listening...\n");
	
	//accept client connected
	memset(&client,0,sizeof(client));
	addrlen = sizeof(struct sockaddr);
	cfd = accept(sfd,(struct sockaddr*)&client,&addrlen);
	if(-1 == cfd)
	{
		perror("accept");
		return -1;
	}
	printf("%d has connected!\n",cfd);
	printf("IP:%s PORT:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	
	//start data changing
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(cfd,&rdset);
		ret = select(cfd+1,&rdset,NULL,NULL,NULL);
		if(ret > 0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				read(0,buf,sizeof(buf));
				ret = send(cfd,buf,strlen(buf)-1,0);
				if(-1 == ret)
				{
					perror("send");
					break;
				}	
				//printf("%d:%s",sfd,buf);
			}
			if(FD_ISSET(cfd,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret = recv(cfd,buf,sizeof(buf),0);
				if(-1 == ret)
				{
					perror("recv");
					break;		
				}
				if(0 == ret)
				{
					printf("the connection of %d has filed\n",cfd);
					break;
				}
				printf("client:%s\n",buf);
			}
		
		}	
	}
	close(cfd);	
	close(sfd);
	return 0;
}











