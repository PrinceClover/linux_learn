#include"head.h"

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("error args!\n");
		return -1;
	}
	//创建socket
	int cfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == cfd)
	{
		perror("socket");
		return -1;
	}
	//链接服务器
	int addrlen;
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	int ret = connect(cfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("connect");
		return -1;
	}
	printf("connected!start receiving data.\n");
	//接收数据
	data recvbuf;
	memset(&recvbuf,0,sizeof(recvbuf));
	//读取文件名并打开文件
	recv_n(cfd,&recvbuf.len,sizeof(recvbuf.len));
	recv_n(cfd,recvbuf.buf,recvbuf.len);
	int fd = open(recvbuf.buf,O_RDWR|O_CREAT,0666);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}
	//读取文件信息
	while(1)
	{
		memset(&recvbuf,0,sizeof(recvbuf));
		recv_n(cfd,&recvbuf.len,sizeof(recvbuf.len));
		if(recvbuf.len > 0)
		{
			recv_n(cfd,recvbuf.buf,recvbuf.len);
			write(fd,recvbuf.buf,recvbuf.len);
		}
		else
		{
			break;
		}
	}
	close(cfd);
	close(fd);
	printf("finished!\n");
	return 0;
}

