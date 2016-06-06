#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//function:
//send socket fd to fds
void send_fd(int fds,int fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	char buf1[30] = "send to my parents' file\n";
	char buf2[30] = "do you receive it?=w=\n";
	iov[0].iov_base = buf1;
	iov[0].iov_len = 25;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 23;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr* cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	*(int *)CMSG_DATA(cmsg) = fd;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = sendmsg(fds,&msg,0);
	if(-1 == ret)
	{
		perror("sendmsg");
		return;
	}
}
//function:
//receive fd from fds
void recv_fd(int fds,int *pfd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	char buf1[30]={0};
	char buf2[30]={0};
	iov[0].iov_base=buf1;
	iov[0].iov_len = 25;
	iov[1].iov_base=buf2;
	iov[1].iov_len = 23;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr* cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret=recvmsg(fds,&msg,0);
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}
	*pfd = *(int*)CMSG_DATA(cmsg);
}
int main()
{
	//variates definition
	int fds[2];			//reserve socket
	int ret;			//receive return value
	//create a pair of socket
	ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
	if(-1 == ret)
	{
		perror("socketpair");
		return -1;
	}
	//start child progress
	if(!fork())
	{
		//child progress
		close(fds[0]);
		int fd;
		recv_fd(fds[1],&fd);
		printf("CHILD:my fd is %d\n",fd);
		char buf[128]={0};
		read(fd,buf,sizeof(buf));
		printf("CHILD:%s",buf);
		exit(0);
	}
	else
	{
		//parent progress
		close(fds[1]);
		int fd1 = open("file",O_RDWR);
		printf("PARENT:%d\n",fd1);
		send_fd(fds[0],fd1);
		printf("PARENT:wait...\n");
		wait(NULL);
		return 0;
	}
}
