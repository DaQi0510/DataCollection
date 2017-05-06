#ifndef __EC20_H
#define __EC20_H 	
#include "stm32f10x.h"
#include "misc.h"




void EC20_Init(void);
void USART1_Send_Byte(u16 Data);
void USART1_Send_Information(u8 *Information,u16 Length);
void SendAT(u8 ATNum);
u8 StringToArray(u8 *p);
u8 HTTP_Init(void);
u8 SendPower();
void ChangeToChar(u8 *Informations,u16 Length,u16 InfLength);
u8  WaitReady(void);
u8 WaitOK(void);
u8 WaitConnect(void) ;
u8 WaitGetRespone(void);
#endif