#include "EC20.h"
#include "delay.h"
#include "HardWare.h"
#include "stm32f10x_usart.h"
#include <string.h>
#include <stdlib.h> 
#include "stm32f10x.h"
#include "RS485.h"
extern volatile u8 AT_Message[700];
extern volatile u16 USART_RX_STA;       //����״̬���	
extern u8 Information[700];
extern volatile u16 URL_Len;
u16 k=0;
u8 Next=0;
u8 NextAt[2];
extern u8 Meters_Data_Rec[200];
extern volatile u16 Meters_DataNum;

/************************************
1   ATI
2   AT+CPIN?   //��ѯSIM��PIN��״̬
3   AT+CSQ     //��ѯ�����ź�ǿ��
4   AT+QHTTPCFG=��contextid",1       //Set the PDP context ID as 1
5   AT+QHTTPCFG="responseheader",1  //�������HTTPӦ���ͷ
6   AT+QIACT?                       //�г��ı�״̬
7   AT+QIACT=1                      //�����ı�״̬
8   AT+QHTTPURL=56,80               //���÷�����URL��Ϣ
9   AT+QHTTPGET=80                  //����GETָ��
10  AT+QHTTPREAD=80                 //��ȡGET���ذ�ͷ
100 ATD15933966619; //CALL
************************************/
u8 AT1 [5] ={'A','T','I',0x0D,0x0A}; 
u8 AT2 [10]={'A','T','+','C','P','I','N','?',0x0D,0x0A};
u8 AT3 [8] ={'A','T','+','C','S','Q',0x0D,0x0A};
u8 AT4 [27]={'A','T','+','Q','H','T','T','P','C','F','G','=','"','c','o','n','t','e','x','t','i','d','"',',','1',0x0D,0x0A};
u8 AT5 [32]={'A','T','+','Q','H','T','T','P','C','F','G','=','"','r','e','s','p','o','n','s','e','h','e','a','d','e','r','"',',','1',0x0D,0x0A};
u8 AT6 [11]={'A','T','+','Q','I','A','C','T','?',0x0D,0x0A};
u8 AT7 [12]={'A','T','+','Q','I','A','C','T','=','1',0x0D,0x0A};
u8 AT8 [20]={'A','T','+','Q','H','T','T','P','U','R','L','=','8','7',',','8','0',0x0D,0x0A};
u8 AT9 [16]={'A','T','+','Q','H','T','T','P','G','E','T','=','8','0',0x0D,0x0A};
u8 AT10[17]={'A','T','+','Q','H','T','T','P','R','E','A','D','=','8','0',0x0D,0x0A};
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
	u16 RxNum;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
  {
		Res=USART_ReceiveData(USART1); //��������
		if(USART_RX_STA>=2)
		{
			if((USART_RX_STA&0x8000)==0)//����δ���
			{
				if(USART_RX_STA&0x4000)//���յ���0x0d
				{
					if(Res==0x0A)
					  USART_RX_STA|=0x8000;	//��������� 
					else 
						USART_RX_STA&=0X3FFF;
				}
				else //��û�յ�0X0D
				{	
					if(Res==0x0d)
						USART_RX_STA|=0x4000;
					else
					{
						RxNum=(USART_RX_STA&0x3ff)-2;
						AT_Message[RxNum]=Res ;
						USART_RX_STA++;
					}
				}
			}
			else       //������ɵ����к�������
			{
				Next++;
				if(Res==0x0d)
				{
					NextAt[0]=Next;
				}
				if(Res==0x0a)
				{
					NextAt[1]=Next;
				}
				if(NextAt[1]-NextAt[0]==1)
				{
					USART_RX_STA=2;
				}
			}
		}	
		if((USART_RX_STA==0)&&(Res==0x0D))   //��ȷ��ͷ
			USART_RX_STA=1;		
		if((USART_RX_STA==1)&&(Res==0x0A))   //��ȷ��ͷ
			USART_RX_STA=2;
	}
	
}
void USART1_Send_Information(u8 *Informations,u16 Length)
{
	u16 i;
	for(i=0;i<Length;i++)
	{
		USART1_Send_Byte(Informations[i]);
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
		case 4:
			USART1_Send_Information(AT4,27);
			break;
		case 5:
			USART1_Send_Information(AT5,32);
			break;
		case 6:
			USART1_Send_Information(AT6,11);
			break;
		case 7:
			USART1_Send_Information(AT7,12);
			break;
		case 9:
			USART1_Send_Information(AT9,16);
			break;
		case 10:
			USART1_Send_Information(AT10,17);
			break;
		case 100:
			USART1_Send_Information(AT100,17);
			break;
		default:
			break;
	}
}
u8 SetUrlLength(u16 Length)   //����URL�ַ����ȣ�����AT8���ݳ���
{
	u8 i,j=12;
	Length=48+Length;
	URL_Len=Length;
	if(Length>99)   //3λ��
	{
		i=Length/100+48;
		AT8[j++]=i;
		i=(Length%100)/10+48;
		AT8[j++]=i;
		i=Length%10+48;
		AT8[j++]=i;
	}
	if(Length>9&&Length<100) //2λ��
	{
		i=Length/10+48;
		AT8[j++]=i;
		i=Length%10+48;
		AT8[j++]=i;
	}
	if(Length<10)
	{
		i=Length%10+48;
		AT8[j++]=i;
	}
	AT8[j++]=',';
	AT8[j++]=8+48;
	AT8[j++]=0+48;   //80S
	AT8[j++]=0x0D;
	AT8[j++]=0x0A;
	return j;
}
u16 SetURL(u8 *Inf,u16 Length)    //������Ҫ���͵����ݼ���λ��������URL����
{
	u16 Len,i;
	Len=StringToArray("http://118.89.221.15:8080/getMess/Rece?message=");
	for(i=0;i<Length;i++)
	{
		Information[i+Len]=Inf[i];
	}
	return (i+Len);
}
u8 HTTP_Init(void)  //����Ϊ0ʱ��ʾ��ʼ���ɹ�
{
	u8 i;
	SendAT(2);    //�������״̬
	i=WaitOK();
	if(i!=0)
		return 1;
	SendAT(4);    //Set the PDP context ID as 1
	i=WaitOK();
	if(i!=0)
		return 2;
	SendAT(5);    //�������HTTPӦ���ͷ
	i=WaitOK();
	if(i!=0)
		return 3;
	SendAT(6);    //�г��ı�״̬
	i=WaitOK();
	if(i!=0)
		return 4;
	delay_ms(500);
  SendAT(7);    //�����ı�״̬
	i=WaitOK();
	if(i!=0)
		return 5;
	SendAT(6);    //�г��ı�״̬
	i=WaitOK();
	if(i!=0)
		return 6;
	return 0;
}
u8 SendPower()    //����0����ʾ���ͳɹ�
{
	u8 i,j;
	i=GetAPower();
	if(i!=0)
		return 1;
	j=SetUrlLength(40);
	USART1_Send_Information(AT8,j);
	i=WaitConnect();
	if(i!=0)
		return 2;
	StringToArray("http://118.89.221.15:8080/Receiver/test?message=");
	ChangeToChar(Meters_Data_Rec,20,48);
//	SetURL(Meters_Data_Rec,20);
	USART1_Send_Information(Information,88);
	i=WaitOK();
	if(i!=0)
		return 3;
	delay_ms (200);
	SendAT(9);
	i=WaitGetRespone();
	if(i!=0)
		return 4;
	SendAT(10);
		i=WaitGetRespone();
	if(i!=0)
		return 5;
	return 0;
//  u8 i,j;
//	i=GetAPower();
//	USART1_Send_Information(AT8,19);
//	delay_ms (1000);
//	USART_RX_STA=0;
	
//	for(i=0;i<20;i++)
//	{
//		Information[47+i]=i+65;
//	}
//	for(i=0;i<10;i++)
//	{
//		Information[57+i]=i+48;
//	}
//	USART1_Send_Information(Information,87);
//	delay_ms(1000);
//	USART_RX_STA=0;
//	SendAT(9);
//	delay_ms(1000);
//	USART_RX_STA=0;
//	SendAT(10);
}
u8 StringToArray(u8 *p)    //����������ַ���������������ַ�������
{
	u16 i=0;
	while(*p!='\0')//�ж��ǲ��ǷǷ��ַ�!
	{
		Information[i]=*p;
		p++;
		i++;
	}
	return i;
}
u8  WaitReady(void)   //�ȴ�EC20��ʼ�����,����0��ʼ���ɹ�
{
	while((USART_RX_STA & 0x8000)==0)
	{
	}
	if((AT_Message[0]=='R')&&(AT_Message[1]=='D')&&(AT_Message[2]=='Y'))
	{
		USART_RX_STA=0;
		AT_Message[0]=0;
		AT_Message[1]=0;
		AT_Message[2]=0;
		return 0;	
	}
	else 
	{
		USART_RX_STA=0;
		return 1;
	}
}
u8 WaitOK(void)
{
	while((USART_RX_STA & 0x8000)==0)
	{
	}
	delay_ms(20);
	if((AT_Message[0]=='O')&&(AT_Message[1]=='K'))
	{
		USART_RX_STA=0;
		AT_Message[0]=0;
		AT_Message[1]=0;
		return 0;	
	}
	else 
	{
		USART_RX_STA=0;
		return 1;
	}
}

u8 WaitConnect(void)   //�ȴ������ź�
{
	while((USART_RX_STA & 0x8000)==0)
	{
	}
	delay_ms(20);
	if((AT_Message[0]=='C')&&(AT_Message[1]=='O')&&(AT_Message[2]=='N')&&(AT_Message[3]=='N')&&(AT_Message[4]=='E'))
	{
		USART_RX_STA=0;
		AT_Message[0]=0;
		AT_Message[1]=0;
		AT_Message[2]=0;
		AT_Message[3]=0;
		AT_Message[4]=0;
		AT_Message[5]=0;
		return 0;	
	}
	else 
	{
		USART_RX_STA=0;
		return 1;
	}
}
u8 WaitGetRespone(void)  //�ȴ�http�����ź�
{
	while((USART_RX_STA & 0x8000)==0)
	{
	}
	delay_ms(800);
	delay_ms(800);
	if((AT_Message[0]=='+')&&(AT_Message[1]=='Q')&&(AT_Message[2]=='H')&&(AT_Message[3]=='T')&&(AT_Message[4]=='T'))
	{
		USART_RX_STA=0;
		AT_Message[0]=0;
		AT_Message[1]=0;
		AT_Message[2]=0;
		AT_Message[3]=0;
		AT_Message[4]=0;
		AT_Message[5]=0;
		return 0;	
	}
	else 
	{
		USART_RX_STA=0;
		return 1;
	}
}
void ChangeToChar(u8 *Informations,u16 Length,u16 InfLength)
{
	u16 i;
	for(i=0;i<Length;i++)
	{
		Information[InfLength]=Informations[i]/16;
		if(Information[InfLength]>9)
			Information[InfLength]+=55;
		else
			Information[InfLength]+=48;
		InfLength++;
		Information[InfLength]=Informations[i]%16;
		if(Information[InfLength]>9)
			Information[InfLength]+=55;
		else
			Information[InfLength]+=48;
		InfLength++;
	}
}