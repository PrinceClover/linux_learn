#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>
#include<sys/types.h>

#define MAXCONN 10		//max connections 
#define BUFLEN 	128		//buffer length

