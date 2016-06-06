#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("args error!\n");
		return -1;
	}
	int i = atoi(argv[1]);
	int j = atoi(argv[2]);
	printf("multi=%d\n",multi(i,j));
	return 0;
}
