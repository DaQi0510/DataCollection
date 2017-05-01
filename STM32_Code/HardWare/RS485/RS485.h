#ifndef __RS485_H
#define __RS485_H 	

#include "stm32f10x.h"
#include "misc.h"
#include "delay.h"
#include "HardWare.h"
#include "stm32f10x_usart.h"

void RS485_Init(void);
void USART2_Send_Byte(u16 Data);
void USART2_Send_Information(u8 *Information,u16 Length);


#endif
