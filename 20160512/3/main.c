#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("error args!\n");
		return -1;
	}
	int a,b,s;
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	s = sum(a,b);
	printf("sum = %d\n",s);
	return 0;
}
