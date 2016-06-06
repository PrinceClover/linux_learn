/*
 *name:pp_order.c  // achieve function in pp_order(client)
 *author@prince_clover
 *date:2016-06-02
 *last edit date:
 */
#include "../include/pp_order.h"

void handleOrder(porder po,char* path,int cfd)
{
//	printf("path:%s\n",path);
	if(ORDER_PWD == po->order)
	{
		data tbuf;
		memset(&tbuf,0,sizeof(tbuf));
		recvN(cfd,(char*)&tbuf.len,sizeof(tbuf.len));
		recvN(cfd,tbuf.buf,tbuf.len);
		printf("%s\n",tbuf.buf);
	}
	else if(ORDER_LS == po->order)
	{
		data tbuf;
		memset(&tbuf,0,sizeof(tbuf));
		recvN(cfd,(char*)&tbuf.len,sizeof(tbuf.len));
		recvN(cfd,tbuf.buf,tbuf.len);
		printf("%-5s%-20s%-10s\n","type","name","size");
		printf("%s\n",tbuf.buf);
	}
	else if(ORDER_CD == po->order)
	{
		data tbuf;
		memset(&tbuf,0,sizeof(tbuf));
		recvN(cfd,(char*)&tbuf.len,sizeof(tbuf.len));
		recvN(cfd,tbuf.buf,tbuf.len);
		printf("%s\n",tbuf.buf);
	}
	else if(ORDER_PUTS == po->order)
	{
		sendFile(cfd,path,po->info);
	}
	else if(ORDER_GETS == po->order)
	{
		recvFile(cfd,path,po->info);
	}
	else if(ORDER_RM == po->order)
	{
		data tbuf;
		memset(&tbuf,0,sizeof(tbuf));
		tbuf.len = strlen(po->info);
		strcpy(tbuf.buf,po->info);
		sendN(cfd,(char*)&tbuf,tbuf.len+sizeof(tbuf.len));
		memset(&tbuf,0,sizeof(tbuf));
		recvN(cfd,(char*)&tbuf.len,sizeof(tbuf.len));
		recvN(cfd,tbuf.buf,tbuf.len);
		printf("%s\n",tbuf.buf);
	}
	else if(ORDER_EXIT == po->order)
	{
		printf("bye bye!\n");
		close(cfd);
		exit(0);
	}
	else
	{
		printf("command not found\n");
	}
}

void sendOrder(int fdw,porder po)
{
	data sbuf;
	memset(&sbuf,0,sizeof(sbuf));
	po->infolen = strlen(po->info);
	readOrder(po->info,po->infolen,po);

	//send order to server
	sbuf.len = sizeof(*po);
	memcpy(sbuf.buf,(char*)po,sbuf.len);	
	sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
//	printf("len = %d\nbuf = %s\n",sbuf.len,sbuf.buf);
}

void readOrder(char* orderinfo,int infolen,porder pf)
{
	char order[10]={'0'};
	int ret = sscanf(orderinfo,"%s%s",order,pf->info);
	if(1 == ret)
	{
		memset(pf->info,0,sizeof(pf->info));
		pf->infolen = 0;
	}
	else
	{
		pf->infolen = strlen(pf->info);
	}
//	printf("ret:%d order:%s info:%s len:%d\n",ret,order,pf->info,pf->infolen);
	if(!strcmp(order,"pwd"))
		pf->order = ORDER_PWD;
	else if(!strcmp(order,"ls"))
		pf->order = ORDER_LS;
	else if(!strcmp(order,"cd"))
		pf->order = ORDER_CD;
	else if(!strcmp(order,"puts"))
		pf->order = ORDER_PUTS;
	else if(!strcmp(order,"gets"))
		pf->order = ORDER_GETS;
	else if(!strcmp(order,"rm"))
		pf->order = ORDER_RM;
	else if(!strcmp(order,"exit"))
		pf->order = ORDER_EXIT;
	else
		pf->order = ORDER_ERROR;
}
