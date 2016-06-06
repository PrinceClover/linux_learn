#include"head.h"
	
int main(int argc,char** argv)
{
	//variate definition
	int sfd;					//socket descriptors
	struct sockaddr_in client;	//connection information of client;
	int ret;					//receive return value
	char buf[BUFLEN];			//buffer
	fd_set rdset;				//read set
	int addrlen;				//connection information length	

	//check args
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

	//connect server
	memset(&client,0,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(argv[1]);
	client.sin_port = htons(atoi(argv[2]));
	ret = connect(sfd,(struct sockaddr*)&client,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("connect");
		return -1;
	}	
	printf("%d has connected server!\n",sfd);
	
	//start data changing
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(sfd,&rdset);
		ret = select(sfd+1,&rdset,NULL,NULL,NULL);
		if(ret > 0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret = read(0,buf,sizeof(buf));
				if(ret <= 0)
				{
					perror("read");
					break;
				}
				ret = send(sfd,buf,strlen(buf)-1,0);
				if(-1 == ret)
				{
					perror("send");
					break;
				}
			//	printf("%d:%s",sfd,buf);
			}
			if(FD_ISSET(sfd,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret = recv(sfd,buf,sizeof(buf),0);
				if(-1 == ret)
				{
					perror("recv");
					break;
				}
				if(0 == ret)
				{
					printf("the connection of server has filed\n");
					break;
				}
				printf("server:%s\n",buf);
			}
		}
	}	
	close(sfd);
	return 0;
}







	


