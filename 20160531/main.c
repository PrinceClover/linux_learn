#include "factory.h"

int main(int argc,char* argv[])
{
	//check args
	if(argc != 5)
	{
		printf("error args!\n");
		return -1;
	}
	//initialize fortory
	factory fact;
	factory_init(&fact,atoi(argv[3]),atoi(argv[4]),child_thread);
	factory_start(&fact);
	printf("system startup!\n");
	//startup server//
	//create socket
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	//bind ip address and port
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	int ret = bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		return -1;
	}
	ret = listen(sfd,atoi(argv[3]));
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	printf("listening...\n");
	//handle accept from client
	int rfd;
	while(1)
	{
		rfd = accept(sfd,NULL,NULL);
		if(-1 == rfd)
		{
			perror("accept");
			return -1;
		}
		que_enque(&fact.que,rfd);
		pthread_cond_signal(&fact.cond);
	}
	return 0;
}







