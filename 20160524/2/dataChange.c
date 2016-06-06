#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("args error!\n");
		return -1;
	}
	unsigned short port;
	port = atoi(argv[1]);
	printf("host:%d(%x) -> net:%x\n",port,port,htons(port));
	printf("ip:%s -> ip:%x\n",argv[2],inet_addr(argv[2]));
	return 0;
}
