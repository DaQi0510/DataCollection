#ifndef __EC20_H
#define __EC20_H 	
#include "stm32f10x.h"
#include "misc.h"




void EC20_Init(void);
void USART1_Send_Byte(u16 Data);
void USART1_Send_Information(u8 *Information,u16 Length);
void SendAT(u8 ATNum);
#endif