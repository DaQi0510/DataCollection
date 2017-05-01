#include "EC20.h"
#include "delay.h"
#include "HardWare.h"
#include "stm32f10x_usart.h"
extern volatile u8 Inf;
extern volatile u8 Message[100];
/************************************
1   ATI
2   AT+CPIN?   //��ѯSIM��PIN��״̬
3   AT+CSQ
100 ATD15933966619; //CALL
************************************/
u8 AT1[5] ={'A','T','I',0x0D,0x0A}; 
u8 AT2[10]={'A','T','+','C','P','I','N','?',0x0D,0x0A};
u8 AT3[8]={'A','T','+','C','S','Q',0x0D,0x0A};
u8 AT100[17]={'A','T','D','1','5','9','3','3','9','6','6','6','1','9',';',0x0D,0x0A};
void EC20_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	/****************TX*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************RX*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  /****************NVIC*******************/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	/****************UART*******************/
	USART_InitStructure.USART_BaudRate = 115200;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
	/****************RESET*******************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	EC20_RESET=0;
  /****************WAKEUP_OUT*******************/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/****************WAKEUP_IN��W_DISABLE*******************/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA�˿�ʱ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;				 //LED0-->PA.8 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	EC20_WAKEUP_IN=1;
//	EC20_W_DISABLE=1;
//	EC20_RESET=0;
	delay_ms(1000);
	EC20_RESET=1;
	
}
void USART1_Send_Byte(u16 Data)
{ 
  while((USART1->SR&0X40)==0); 
	USART1->DR = (Data & (uint16_t)0x01FF);	 
  while((USART1->SR&0X40)==0);  
}
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
  {
		Message[Inf] =USART_ReceiveData(USART1); //��������
		
	}
	Inf++;
}
void USART1_Send_Information(u8 *Information,u16 Length)
{
	u16 i;
	for(i=0;i<Length;i++)
	{
		USART1_Send_Byte(Information[i]);
	}
}
void SendAT(u8 ATNum)
{
	switch(ATNum)
	{
		case 1:
			USART1_Send_Information(AT1,5);
			break;
		case 2:
			USART1_Send_Information(AT2,10);
			break;
		case 3:
			USART1_Send_Information(AT3,8);
			break;
		case 100:
			USART1_Send_Information(AT100,17);
			break;
		default:
			break;
	}
}