#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/epoll.h>	
#include<errno.h>

#define MAXCONN 10		//max connections 
#define BUFLEN 16		//buffer length

