/*
 *name:pp_order.c  // achieve function in pp_order(server)
 *author@prince_clover
 *date:2016-06-02
 *last edit date:
 */
#include "../include/pp_order.h"

void handleOrder(porder pf,char* nowpath,int rfd,char* rootpath)
{
	if(ORDER_PWD == pf->order)
	{
		orderPwd(nowpath,rfd);
	}
	else if(ORDER_LS == pf->order)
	{
		orderLs(nowpath,rfd);
	}
	else if(ORDER_CD == pf->order)
	{
		orderCd(nowpath,rfd,pf->info,rootpath);
	}
	else if(ORDER_PUTS == pf->order)
	{
		recvFile(nowpath,pf->info,rfd);
	}	
	else if(ORDER_GETS == pf->order)
	{
		sendFile(nowpath,pf->info,rfd);	
	}
	else if(ORDER_RM == pf->order)
	{
		orderRm(nowpath,rfd);
	}
	else if(ORDER_EXIT == pf->order)
	{
		//printf("client had exit\n");
	}
	else
	{
		//printf("error\n");
	}
}

void orderRm(char* nowpath,int rfd)
{
	data tbuf;
	char buf[O_MAX_BUF];
	struct stat st;
	memset(&tbuf,0,sizeof(tbuf));
	memset(buf,0,sizeof(buf));
	memset(&st,0,sizeof(st));
	recvN(rfd,(char*)&tbuf.len,sizeof(tbuf.len));
	recvN(rfd,tbuf.buf,tbuf.len);
	sprintf(buf,"%s%s%s",nowpath,"/",tbuf.buf);
	char name[O_MAX_BUF];
	strcpy(name,tbuf.buf);
	int ret = stat(buf,&st);
	memset(&tbuf,0,sizeof(tbuf));
	if(-1 == ret)
	{
		sprintf(tbuf.buf,"can not find file [%s]",name);
		tbuf.len = strlen(tbuf.buf);
		sendN(rfd,(char*)&tbuf,tbuf.len+sizeof(tbuf.len));
		return;
	}
	if(st.st_mode & S_IFDIR)
	{
		sprintf(tbuf.buf,"[%s] is a dir",name);
		tbuf.len = strlen(tbuf.buf);
		sendN(rfd,(char*)&tbuf,tbuf.len+sizeof(tbuf.len));
		return;	
	}
	ret = unlink(buf);
	if(-1 == ret)
	{
		perror("unlink");
		sprintf(tbuf.buf,"[%s] remove failed",name);
		tbuf.len = strlen(tbuf.buf);
		sendN(rfd,(char*)&tbuf,tbuf.len+sizeof(tbuf.len));
		return;
	}
	else
	{
		sprintf(tbuf.buf,"[%s] remove complete",name);
		tbuf.len = strlen(tbuf.buf);
		sendN(rfd,(char*)&tbuf,tbuf.len+sizeof(tbuf.len));
		return;	
	}
}

void orderCd(char* nowpath,int fdw,char* info,char* rootpath)
{	
	int infolen = strlen(info);
	int pathlen = strlen(nowpath);
	int rootlen = strlen(rootpath);
	char tpath[O_MAX_BUF];
	memset(tpath,0,sizeof(tpath));
	strcpy(tpath,nowpath);
//	printf("rootpath:%s nowpath:%s info:%s infolen:%d pathlen:%d rootpath:%d\n",rootpath,nowpath,info,infolen,pathlen,rootlen);
	int i,j;
	for(i = 0;i < infolen;i++)
	{
		if(info[i] == '.')
		{
			if(info[i+1] == '.')
			{
				if(!strcmp(tpath,rootpath))
					continue;
				else
				{
					for(j = pathlen-1;j >= 0;j--)
					{
						if(tpath[j] =='/')
						{
							tpath[j] ='\0';
							break;
						}
					}
				}
			}
			else
			{
				continue;
			}
		}
		else 
		{
			if(info[i] == '/')
			{
				if(info[i+1] == '/')
					continue;
				else
				{		
					sprintf(tpath,"%s%s",tpath,info+i);
					break;
				}
			}
			else
			{
				sprintf(tpath,"%s%s%s",tpath,"/",info+i);
				break;
			}
		}
	}
	if(NULL == opendir(tpath))
	{
		data sbuf;
		memset(&sbuf,0,sizeof(sbuf));
		sprintf(sbuf.buf,"[%s] not a dir!",info);
		sbuf.len = strlen(sbuf.buf);
		sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
//		printf("not a dir\n");
	}
	else
	{
		strcpy(nowpath,tpath);
		orderPwd(nowpath,fdw);
	}
//	printf("rootpath:%s nowpath:%s info:%s\n",rootpath,nowpath,info);
}

void orderLs(char* path,int fdw)
{
	DIR *dir = opendir(path);
	if(NULL == dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	char buf[O_MAX_BUF];
	char newpath[O_MAX_BUF];
	struct stat infobuf;
	data tbuf;
	memset(&tbuf,0,sizeof(tbuf));
	int size = 0;
	int flag = 0;
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
		{
			continue;
		}
		memset(&infobuf,0,sizeof(infobuf));
		memset(newpath,0,sizeof(newpath));
		sprintf(newpath,"%s%s%s",path,"/",p->d_name);
//		printf("%s\n",newpath);
		int ret = stat(newpath,&infobuf);
		if(-1 == ret)
		{
			perror("opendir");
			return;
		}		
		if(infobuf.st_mode & S_IFDIR)
			strcpy(newpath,"d");
		else
			strcpy(newpath,"-");
		memset(buf,0,sizeof(buf));	
		if(0 == flag)
		{
			sprintf(buf,"%-5s%-20s%-10ld",newpath,p->d_name,infobuf.st_size);
			flag = 1;
		}
		else
			sprintf(buf,"\n%-5s%-20s%-10ld",newpath,p->d_name,infobuf.st_size);
		strcpy(tbuf.buf+size,buf);
		size += strlen(buf);
//		printf("%s\nret=%d\n",tbuf.buf,ret);
	}
	closedir(dir);
	tbuf.len = size;
//	printf("tbuflen:%d\n",tbuf.len);
	sendN(fdw,(char*)&tbuf,tbuf.len+sizeof(tbuf.len));
}


void orderPwd(char* path,int fdw)
{
	data tbuf;
	memset(&tbuf,0,sizeof(tbuf));
	tbuf.len = strlen(path);
	strcpy(tbuf.buf,path);
	sendN(fdw,(char*)&tbuf,tbuf.len+sizeof(tbuf.len));
}

/*
void changeDirS2C(char* spath,char* cpath)
{
	int slen = strlen(spath);
	int clen = strlen(cpath);
	int i,j;
	for(i = slen - 1;i >= 0;i--)
	{
		
	}
}
*/
void recvOrder(int fdr,porder pf)
{
	data sbuf;
	memset(&sbuf,0,sizeof(sbuf));
	//recv order form client
	recvN(fdr,(char*)&sbuf.len,sizeof(sbuf.len));
	recvN(fdr,sbuf.buf,sbuf.len);
	memcpy((char*)pf,sbuf.buf,sbuf.len);
//	pf->infolen = strlen(pf->info);
//	readOrder(pf->info,pf->infolen,pf);	
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
