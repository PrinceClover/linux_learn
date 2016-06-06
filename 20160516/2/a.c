#include"head.h"

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}
	int fdw = open(argv[1],O_WRONLY);
	if(-1 == fdw)
	{
		perror("open_1");
		return -1;
	}
	int fdr = open(argv[2],O_RDONLY);
	if(-1 == fdw)
	{
		perror("open_2");
		return -1;
	}
	printf("fdr=%d,fdw=%d\n",fdr,fdw);
	char buf[128];
	fd_set rdset;
	int ret;
	int ret1;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		ret = select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret > 0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				read(0,buf,sizeof(buf));
				write(fdw,buf,strlen(buf)-1);
			}
			if(FD_ISSET(fdr,&rdset))
			{
				bzero(buf,sizeof(buf));		
				ret1 = read(fdr,buf,sizeof(buf));
				if(ret1 == 0)
				{
					printf("b has exited\n");
					break;
				}
				printf("a:%s\n",buf);
			}
		}
	}
	return 0;
}
