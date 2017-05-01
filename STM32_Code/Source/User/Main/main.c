#include "stm32f10x.h"
#include "EC20.h"
#include "delay.h"
#include "lcd.h"
#include "RS485.h"
#include "W25Q64.h"
volatile u8 Inf;
volatile u8 Message[100];
volatile u16 Meters_DataNum=0;
u8 Meters_Data_Send[200];
u8 Meters_Data_Rec[200];
u8 Address[7];
u16 ERRORNUM;
u8 as1[200];
u8 as2[200];
int main(void)
{
	u8 i;
	u16 ID;
	SystemInit();	   	
	delay_init(72);
	W25Q64_Init();
	LCD_Init();
	Meters_Data_Send[0]=0x00;
	Meters_Data_Send[1]=0x00;
	Meters_Data_Send[2]=0x00;
	Meters_Data_Send[3]=0x00;
	Meters_Data_Send[4]=0x68;
	Meters_Data_Send[5]=0xAA;
	Meters_Data_Send[6]=0xAA;
	Meters_Data_Send[7]=0xAA;
	Meters_Data_Send[8]=0xAA;
	Meters_Data_Send[9]=0xAA;
	Meters_Data_Send[10]=0xAA;
	Meters_Data_Send[11]=0x68;
	Meters_Data_Send[12]=0x13;
	Meters_Data_Send[13]=0x00;
	Meters_Data_Send[15]=0x16;
	ID=W25QXX_ReadID();
//	LCD_ShowNum(0,0,ID/256);
//	LCD_ShowNum(0,1,ID%256);
	for(i=4;i<13;i++)
	{
		Meters_Data_Send[14]+=Meters_Data_Send[i];
	}
	for(i=0;i<200;i++)
	{
		as1[i]=i;
	}

	W25QXX_Write(as1,0x00,200);
	W25QXX_Read(as2,0x00,200);
	EC20_Init();
	
	RS485_Init();
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	SendAT(3);
	while(1)
	{
		LCD_ShowNum(0,0,Inf);
		delay_ms(1000);
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
		USART2_Send_Information(Meters_Data_Send,16);
		delay_ms(1000);
		if(Address[6]==0)
		{
			for(i=0;i<6;i++)
				Address[i]=Meters_Data_Rec[1+i];
		}
		else
		{
			for(i=0;i<6;i++)
			{
				if(Address[i]!=Meters_Data_Rec[1+i])
					break;
			}
			if(i!=6)
				ERRORNUM++;
		}
		LCD_ShowNum(0,4,ERRORNUM);
		LCD_ShowNum(0,3,Meters_DataNum);
		
		delay_ms(1000);
	}
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


