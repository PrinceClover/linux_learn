#include"head.h"

void send_fd(int fds,int fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	char buf1[10]="hello";
	char buf2[10]="world";
	iov[0].iov_base = buf1;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr* cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg) = fd;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = sendmsg(fds,&msg,0);
	if(-1 == ret)
	{
		perror("sendmsg");
		exit(-1);
	} 
}

void recv_fd(int fds,int *pfd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	char buf1[10]={0};
	char buf2[10]={0};
	iov[0].iov_base = buf1;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr* cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = recvmsg(fds,&msg,0);
	if(-1 == ret)
	{
		perror("recvmsg");
		exit(-1);
	}
	*pfd = *(int*)CMSG_DATA(cmsg);
}

void send_file(int fdw)
{
	data sendbuf;
	memset(&sendbuf,0,sizeof(sendbuf));
	//发送文件名
	sendbuf.len = strlen(FILENAME);
	strcpy(sendbuf.buf,FILENAME);
	int ret = send(fdw,&sendbuf,sendbuf.len+sizeof(sendbuf.len),0);
	if(-1 == ret)
	{
		perror("send 111");
		exit(-1);
	}
	//发送文件
	int fd = open(FILENAME,O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		exit(-1);
	}
	while(memset(&sendbuf,0,sizeof(sendbuf)),(sendbuf.len=read(fd,sendbuf.buf,sizeof(sendbuf.buf))))
	{
		send_n(fdw,&sendbuf,sendbuf.len+sizeof(sendbuf.len));
	}
	//发送文件结束标志
	ret = 0;
	send_n(fdw,&ret,sizeof(ret));
	close(fdw);
}
