/*:
 *name:pp_order.h  // send/recv order headfile(server)
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
#define ORDER_LS  2
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
void orderPwd(char*,int);			//pwd order response fun
void orderLs(char*,int);			//ls order response fun
void orderCd(char*,int,char*,char*);		//cd order response fun
void orderRm(char*,int);			//rm order response fun

//order handle function
void recvOrder(int,porder);			//recv order from client
void readOrder(char*,int,porder);	//read order
void handleOrder(porder,char*,int,char*);		//handle order
void dirFormat(char*,char*);			//path joint
//void changeDirS2C(char*,char*);		//changedir form serverformat to cilent
//void changeDirC2S(char*,char*);		//changedir from cilentformat to serveir

#endif
