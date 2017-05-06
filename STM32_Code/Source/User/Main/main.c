#include "stm32f10x.h"
#include "EC20.h"
#include "delay.h"
#include "lcd.h"
#include "RS485.h"
#include "W25Q64.h"
#include <string.h>
#include <stdlib.h>
#include "timer.h"
volatile u16 USART_RX_STA=0; 
volatile u8 AT_Message[700];
volatile u16 Meters_DataNum=0;
volatile u16 URL_Len=0;
u8 volatile Meters_Data_Rec[100];
u8 MeterAddress[6];
u16 ERRORNUM;
u8 as1[200];
u8 as2[200];
u8 Information[700];   //记录需要发送的信息
volatile u16 TimeFlag=0;
u16 Flag;
int main(void)
{
	u16 i;
	u16 ID;
	SystemInit();	   	
	delay_init(72);
	W25Q64_Init();
	LCD_Init();
	EC20_Init();
	RS485_Init();
	SetMeterAddress();
  Get_MeterAddress();
	SetAPowerData();
//	LCD_ShowString(0,0,"4GStart");
//	while(1)
//	{
//		i++;
//		Flag=GetAPower();
//		LCD_ShowNum(50,0,ERRORNUM);
//		LCD_ShowNum(0,7,i);
//		delay_ms(1000);
//	}
	i=WaitReady();
	delay_ms(1000);
	i=HTTP_Init();
	EC20Start(i);
	Flag =SendPower();
	GetAPower();
	ShoWPowerData(Meters_Data_Rec);
	Timerx_Init(0,0);
	ShoWPowerHead();
	i=0;
	while(1)
	{
		if(TimeFlag>=900)
		{
			TimeFlag=0;
//			LCD_ShowNum(0,5,i);
//			HTTP_Init();
//			LCD_ShowNum(0,6,i);
			Flag=SendPower();
			GetAPower();
			ShoWPowerData(Meters_Data_Rec);
//			LCD_ShowNum(0,5,Flag);
			
			
			i++;
		}
//		LCD_ShowNum(0,4,TimeFlag);
		delay_ms(100);
	}
//  while (1)
//	{
//		LCD_ShowNum(0,0,Inf);
//		LCD_ShowNum(0,1,Message[0]);
//		LCD_ShowNum(0,2,Message[1]);
//		LCD_ShowNum(0,3,Message[3]);
//		USART1_Send_Byte('A');
//		USART1_Send_Byte('T');
//		USART1_Send_Byte('I');
//		USART1_Send_Byte(0x0D);
//		USART1_Send_Byte(0x0A);
//		delay_ms(1000);
//	}
	while(1)
	{
//		Get_MeterAddress();
//		delay_ms(1000);
//		LCD_ShowNum(0,3,Meters_DataNum);
		delay_ms(1000);
	}
//		if(Address[6]==0)
//		{
//			for(i=0;i<6;i++)
//				Address[i]=Meters_Data_Rec[1+i];
//		}
//		else
//		{
//			for(i=0;i<6;i++)
//			{
//				if(Address[i]!=Meters_Data_Rec[1+i])
//					break;
//			}
//			if(i!=6)
//				ERRORNUM++;
//		}
//		LCD_ShowNum(0,4,ERRORNUM);
//		LCD_ShowNum(0,3,Meters_DataNum);
//		
//		delay_ms(1000);
//	}
//  delay_ms(1000);

////  EC20_RESET=1;
//  EC20_Power=0;
//	while (i<5)
//	{
//		LCD_ShowNum(0,0,Inf);
//		LCD_ShowNum(0,1,Message[0]);
//		LCD_ShowNum(0,2,Message[1]);
//		USART1_Send_Byte('A');
//		USART1_Send_Byte('T');
//		USART1_Send_Byte('I');
//		USART1_Send_Byte(0x0D);
//		USART1_Send_Byte(0x0A);
////		if(Inf!=0)
////			EC20_RESET=1;
//		delay_ms(1000);
//		i++;
//	}
//	EC20_Power=0;
//	EC20_RESET=0;
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	EC20_Power=1;
//	delay_ms(30);
//	
//	EC20_RESET=1;

}


