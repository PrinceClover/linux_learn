#include "tranfile.h"

int main(int argc,char* argv[])
{
	//check args
	if(argc != 3)
	{
		printf("error args!\n");
		return -1;
	}
	
	//connecting  server
	int cfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == cfd)
	{
		perror("socket");
		return -1;
	}
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
	printf("connected!start receive data.\n");
	//receive file from server
	data rbuf;
	memset(&rbuf,0,sizeof(rbuf));
	//receive file name;
	recv_n(cfd,&rbuf.len,sizeof(rbuf.len));
	recv_n(cfd,rbuf.buf,rbuf.len);
	int fd = open(FILENAME,O_RDWR|O_CREAT,0666);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}
	//receive file main body
	while(1)
	{
		memset(&rbuf,0,sizeof(rbuf));
		recv_n(cfd,&rbuf.len,sizeof(rbuf.len));
		if(rbuf.len > 0)
		{
			recv_n(cfd,rbuf.buf,rbuf.len);
			write(fd,rbuf.buf,rbuf.len);
		}
		else
		{
			break;
		}
	}
	close(fd);
	close(cfd);
	printf("finished!\n");
	return 0;
}




