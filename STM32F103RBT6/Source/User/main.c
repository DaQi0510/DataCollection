#include "stm32f10x.h"
#include "delay.h"
#include "EC20.h"
volatile u8 Message[100];
int main(void)
{
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();
  delay_init(72);
	EC20_Init();
	USART1->DR='A';
	USART1->DR='T';
	USART1->DR='I';
  while (1)
  {
		
  }
}



