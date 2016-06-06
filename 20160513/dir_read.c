#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>

void dirprint(char *dirname,int space)
{
	DIR *dir = opendir(dirname);
	if(NULL == dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	char buf[512];
	while((p = readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
		{
			continue;
		}
		if(p->d_type == 4)
		{
			printf("%*s%s\n",space," ",p->d_name);
			bzero(buf,sizeof(buf));
			sprintf(buf,"%s%s%s",dirname,"/",p->d_name);
			dirprint(buf,space + 4);
		}else{
			printf("%*s%s\n",space,"",p->d_name);
		}
	}
}

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args!\n");
		return -1;
	}
	printf("%s\n",argv[1]);
	dirprint(argv[1],0);
	return 0;
}
