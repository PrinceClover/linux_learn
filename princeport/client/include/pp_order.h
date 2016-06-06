/*
 *name:pp_order.h  // send/recv order headfile(client)
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#ifndef __PP_ORDER_H__
#define __PP_ORDER_H__
#include "pp_head.h"
#include "pp_tranfile.h"

 /*macro*/
#define MAX_INFO 256
#define O_MAX_BUF 512

//order macro
#define ORDER_PWD 1
#define ORDER_LS 2
#define ORDER_CD 3
#define ORDER_PUTS 4
#define ORDER_GETS 5
#define ORDER_RM 6
#define ORDER_EXIT 7
#define ORDER_ERROR -1

/*struct*/
typedef struct 
{
	int order;				//the number of order
	int infolen;			//info length
	char info[MAX_INFO];	//order info(args or other sth)
}order,*porder;				//order struct

/*function*/
//order response function
//void orderExit();				//exit order response fun

//order handle function
void sendOrder(int,porder);			//send order to server
void readOrder(char*,int,porder);	//read order
void handleOrder(porder,char*,int);		//handle order

#endif
