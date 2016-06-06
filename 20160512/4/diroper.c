#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char* argv[])
{
	if(argc != 2)
	{
		printf("error args!\n");
		return -1;
	}
	DIR *d = NULL;
	d = opendir(argv[1]);
	if(NULL == d)
	{
		perror("opendir");
		return -1;
	}
	struct dirent* di = NULL;
	while((di = readdir(d))!=NULL)
	{
		if(!strcmp(di->d_name,".")||!strcmp(di->d_name,".."))
			continue;
		printf("%s\n",di->d_name);
	}
	closedir(d);
	return 0;
}












