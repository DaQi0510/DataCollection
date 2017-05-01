#include "RS485.h"
u8 AddressFlag=0;
extern volatile u16 Meters_DataNum;
extern u8 Meters_Data_Send[200];
extern u8 Meters_Data_Rec[200];
void RS485_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	/****************TX*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************RX*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************RE*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RS485_TX_EN=0;
  /****************NVIC*******************/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	/****************UART*******************/
	USART_InitStructure.USART_BaudRate = 2400;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	/****************RESET*******************/
}
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
  {
		Res=USART_ReceiveData(USART2); //��������
		if((Res==0x68)&&Meters_DataNum==7)
		{
			AddressFlag=1;
			Meters_Data_Rec[Meters_DataNum]=Res;
			Meters_DataNum++;
		}
		if((Res==0x68)&&AddressFlag==0)   //������ʼλ
		{
			Meters_DataNum=0;
			Meters_Data_Rec[Meters_DataNum]=Res;
			Meters_DataNum++;
		}
		if(Res==0x16)
		{
			Meters_Data_Rec[Meters_DataNum]=Res;
			AddressFlag=0;
		}
		if(Res!=0x68&&Res!=0x16)
		{
			Meters_Data_Rec[Meters_DataNum]=Res;
			Meters_DataNum++;
		}

	}
}
void USART2_Send_Byte(u16 Data)
{ 
	while((USART2->SR&0X40)==0); 
	USART2->DR = (Data & (uint16_t)0x01FF);	 
  while((USART2->SR&0X40)==0);  
}
void USART2_Send_Information(u8 *Information,u16 Length)
{
	u16 i;
	RS485_TX_EN=1;
	for(i=0;i<Length;i++)
	{
		USART2_Send_Byte(Information[i]);
	}
	RS485_TX_EN=0;
}