#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>

void timeFormat(char* time)
{
	char *p_time;
	p_time = time + strlen(time) - 1;
/*	
	char c_year[5] = {*(p_time-4),*(p_time-3),*(p_time-2),*(p_time-1),'\0'};
	if(2015 != atoi(c_year))
	{
		strcpy(time,c_year);
		return;
	}
*/ 
	while(*p_time != ':')
	{
		p_time--;
	}
	*p_time = '\0';
}

void modeFormat(char *c_mode,mode_t mode)
{
	long root[3] = {S_IRUSR,S_IWUSR,S_IXUSR};
	char vflag[5] = {'r','w','x','d','-'};
	int i,j;
	if(mode & S_IFDIR)
	{
		c_mode[0] = vflag[3];
	}
	else
	{
		c_mode[0] = vflag[4];
	}
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(mode & root[j])
			{
				c_mode[i*3+j+1] = vflag[j];	
			}
			else
			{
				c_mode[i*3+j+1] = vflag[4]; 
			}		
			root[j] = root[j] >> 3; 
		}
	}
	c_mode[10] = '\0';
}

char *nameFormat(char *dirname)
{
	char *p;
	p = dirname + strlen(dirname) - 1;
	while(*p != '/')
	{
		p--;
	}
	return ++p;
}

void showOneLs(char* filename)
{
	struct stat buf;
	memset(&buf,0,sizeof(buf));
	int ret = stat(filename,&buf);
	if(-1 == ret)
	{
		perror("stat");
		return;
	}
	char t[128];
	char c_mode[20];
	char name[128];
	strcpy(name,nameFormat(filename));
	strcpy(t,ctime(&buf.st_mtime));
	timeFormat(t);
	modeFormat(c_mode,buf.st_mode);
	printf("%s %2d %s %s %7ld %s %s\n",c_mode,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,t+4,name);	
}

void showLs(char *dirname)
{
	DIR *dir = opendir(dirname);
	if(NULL == dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	char buf[512];
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
		{
			continue;
		}
		bzero(buf,sizeof(buf));
		sprintf(buf,"%s%s%s",dirname,"/",p->d_name);
		showOneLs(buf);
	}
}

int main(int argc,char* argv[])
{
	if(argc != 2)
	{
		printf("error args\n");
		return -1;
	}
	showLs(argv[1]);
	return 0;
}
