#include "lcd.h"
#include "sys.h"
#include "HardWare.h"
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 

#define X_WIDTH 128
#define Y_WIDTH 64
extern u8 volatile Meters_Data_Rec[100];
const u8 F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 }    // horiz lines
};

void LCD_WrDat(u8 data)
{
	u8 i=8;
	//LCD_CS=0;;
	SET_LCD_DC;
	delay_us(1);
  CLR_LCD_SCL;
	delay_us(5);   
  while(i--)
  {
    if(data&0x80)
		{
			SET_LCD_SDA;
			delay_us(1);
		}
    else
		{
			CLR_LCD_SDA;
			delay_us(1);
		}
    SET_LCD_SCL; 
    delay_us(3);            
    CLR_LCD_SCL;
		delay_us(1); 
    data<<=1;    
		delay_us(1);
  }
	//LCD_CS=1;
}

void LCD_WrCmd(u8 cmd)
{
	u8 i=8;
	//LCD_CS=0;;
	CLR_LCD_DC;
	delay_us(1);
  CLR_LCD_SCL;
	delay_us(5);
  while(i--)
  {
    if(cmd&0x80)
		{
			SET_LCD_SDA;
		  delay_us(1);
		}
    else
		{
			CLR_LCD_SDA;
			delay_us(1);
		}
    SET_LCD_SCL;
		delay_us(3);          
    CLR_LCD_SCL;
	  delay_us(1); 
    cmd<<=1;
		delay_us(1);
  } 	
	//LCD_CS=1;
}

void LCD_Set_Pos(u8 x, u8 y)
{ 
  LCD_WrCmd(0xb0+y);
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x01); 
} 

void LCD_Fill(u8 bmp_data)
{
	u8 y,x;
	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(bmp_data);
	}
}

void LCD_CLS(void)
{
	u8 y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(0);
	}
}

void Set_Display_On_Off(unsigned char d)
{
	LCD_WrCmd(0xAE|d);			// Set Display On/Off						
}

void adjust(u8 a)
{
  LCD_WrCmd(a);	//????0x0000~0x003f  
}

void Set_Display_Clock(unsigned char d)
{
	LCD_WrCmd(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	LCD_WrCmd(d);			//   Default => 0x80
}

void Set_Multiplex_Ratio(unsigned char d)
{
	LCD_WrCmd(0xA8);			// Set Multiplex Ratio
	LCD_WrCmd(d);			//   Default => 0x3F (1/64 Duty)
}

void Set_Display_Offset(unsigned char d)
{
	LCD_WrCmd(0xD3);			// Set Display Offset
	LCD_WrCmd(d);			//   Default => 0x00
}

void Set_Charge_Pump(unsigned char d)
{
	LCD_WrCmd(0x8D);			// Set Charge Pump
	LCD_WrCmd(0x10|d);			//   Default => 0x10
}

void SetStartLine(unsigned char d)
{
	LCD_WrCmd(0x40|d);			// Set Display Start Line
}

void SetAddressingMode(unsigned char d)
{
	LCD_WrCmd(0x20);			// Set Memory Addressing Mode
	LCD_WrCmd(d);			//   Default => 0x02
}

void Set_Segment_Remap(unsigned char d)
{
	LCD_WrCmd(0xA0|d);			// Set Segment Re-Map
}

void Set_Common_Remap(unsigned char d)
{
	LCD_WrCmd(0xC0|d);			// Set COM Output Scan Direction
}

void Set_Common_Config(unsigned char d)
{
	LCD_WrCmd(0xDA);			// Set COM Pins Hardware Configuration
	LCD_WrCmd(0x02|d);			//   Default => 0x12 (0x10)
}

void SetContrastControl(unsigned char d)
{
	LCD_WrCmd(0x81);			// Set Contrast Control
	LCD_WrCmd(d);			//   Default => 0x7F
}

void Set_Precharge_Period(unsigned char d)
{
	LCD_WrCmd(0xD9);			// Set Pre-Charge Period
	LCD_WrCmd(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
}

void Set_VCOMH(unsigned char d)
{
	LCD_WrCmd(0xDB);			// Set VCOMH Deselect Level
	LCD_WrCmd(d);			//   Default => 0x20 (0.77*VCC)
}

void Set_Entire_Display(unsigned char d)
{
	LCD_WrCmd(0xA4|d);			// Set Entire Display On / Off
}

void Set_Inverse_Display(unsigned char d)
{
	LCD_WrCmd(0xA6|d);			// Set Inverse Display On/Off
}

void LCD_Init(void)        
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA端口时钟
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;				 //LED0-->PA.8 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  SET_LCD_SCL;
  CLR_LCD_RST;
  delay_ms(50);
  SET_LCD_RST;

  Set_Display_On_Off(0x00);
	delay_ms(10);	  // Display Off (0x00/0x01)
  Set_Display_Clock(0x80);	
	delay_ms(10);		  // Set Clock as 100 Frames/Sec
  Set_Multiplex_Ratio(0x3F);
	delay_ms(10);		// 1/64 Duty (0x0F~0x3F)
  Set_Display_Offset(0x00);
	delay_ms(10);	  // Shift Mapping RAM Counter (0x00~0x3F)
  SetStartLine(0x00);
	delay_ms(10);	      // Set Mapping RAM Display Start Line (0x00~0x3F)
  Set_Charge_Pump(0x04);
	delay_ms(10);		    // Enable Embedded DC/DC Converter (0x00/0x04)
  SetAddressingMode(0x02);
	delay_ms(10);		  // Set Page Addressing Mode (0x00/0x01/0x02)
  Set_Segment_Remap(0x01);
	delay_ms(10);			  // Set SEG/Column Mapping     0x00???? 0x01??
  Set_Common_Remap(0x08);
	delay_ms(10);				  // Set COM/Row Scan Direction 0x00???? 0x08??
  Set_Common_Config(0x10);
	delay_ms(10);		  // Set Sequential Configuration (0x00/0x10)
  SetContrastControl(Brightness);
	delay_ms(10);	// Set SEG Output Current
  Set_Precharge_Period(0xF1);
	delay_ms(10);	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  Set_VCOMH(0x40);		
	delay_ms(10);	        // Set VCOM Deselect Level
  Set_Entire_Display(0x00);
	delay_ms(10);	  // Disable Entire Display On (0x00/0x01)
  Set_Inverse_Display(0x00);
	delay_ms(10);	// Disable Inverse Display On (0x00/0x01)  
  Set_Display_On_Off(0x01);	
	delay_ms(10);	  // Display On (0x00/0x01)
  LCD_Fill(0x00); 
	delay_ms(10);	 
	LCD_Set_Pos(0,0); 
	delay_ms(10);	
} 

void LCD_P6x8Str(u8 x,u8 y,u8 ch[])
{
  u8 i=0,j=0;      
	LCD_Set_Pos(x,y);
	j=ch[0]-32;
	for(i=0;i<6;i++)     
  	  LCD_WrDat(F6x8[j][i]); 
}
void LCD_Show_M(u8 y,unsigned long int num)
{
	u8 num1, i, j, k, c;
	num1=num/65536;
	k=100;
	for(i=0;i<3;i++)
	{
		LCD_Set_Pos(0+6*i,y);
		c=(num1/k)%10+16;
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[c][j]); 
		k/=10;	
	}
	num1=(num%65536)/256;
	k=100;
	for(i=0;i<3;i++)
	{
		LCD_Set_Pos(40+6*i,y);
		c=(num1/k)%10+16;
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[c][j]); 
		k/=10;	
	}
	num1=num%256;
	k=100;
	for(i=0;i<3;i++)
	{
		LCD_Set_Pos(80+6*i,y);
		c=(num1/k)%10+16;
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[c][j]); 
		k/=10;	
	}
}
void LCD_ShowNum(u8 x,u8 y, unsigned int num)
{
	u8 i,j,c=0;
	unsigned int t=1,k=1;
	if(num==0)
	{
		LCD_Set_Pos(x,y);
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[16][j]); 
	}
  else	
	{
	t=num;
	for(i=0;t>0;i++)
	{
		t=t/10;
		k=k*10;	
	}
	if(x+6*i>126)
		x=126-6*i;
	k=k/10;
	while(i>0)
	{
		LCD_Set_Pos(x,y);
		c=(num/k)%10;
		c=c+16;
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[c][j]); 
		k=k/10;
		x=x+6;
		i--;
	}
}
}
void Display(void)
{
	u8 HoldTime[10]={'H','o','l','d',' ','T','i','m','e',':'};
	u8 State[6]={'S','t','a','t','e',':'};
	u8 Address1=5;
	u8 Address2=5;
	u8 Address3=0;
	u8 Address4=0;
	u8 i,j;
	for(i=0;i<10;i++)
	{
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[HoldTime[i]-32][j]); 
		Address1+=6;
	}
	for(i=0;i<6;i++)
	{
		LCD_Set_Pos(Address2,1);
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[State[i]-32][j]); 
		Address2+=6;
	}
	for(i=0;i<8;i++)
	{
		LCD_Set_Pos(Address3,3);
		for(j=0;j<6;j++)     
  	  LCD_WrDat(F6x8[17+i][j]); 
		Address3+=16;
	}
	for(i=0;i<8;i++)
	{
		LCD_Set_Pos(Address4,6);
		if(i==0)
		{
			for(j=0;j<6;j++)     
				LCD_WrDat(F6x8[25+i][j]); 
		}
		if(i>=1)
		{
			for(j=0;j<6;j++)     
				LCD_WrDat(F6x8[17][j]); 
			for(j=0;j<6;j++)     
				LCD_WrDat(F6x8[15+i][j]); 
		}
		Address4+=16;
	}
}
void ShowChannel(u8 Value)
{
	u8 i,j;
	u8 Address3=0;
	u8 Address4=0;
	if(Value!=0)
	{
		for(i=0;i<16;i++)
		{
			if(i<8)
			{
				LCD_Set_Pos(Address3,4);
				for(j=0;j<6;j++) 
				{
					if(i+1==Value)
						LCD_WrDat(F6x8[10][j]); 
					else					
						LCD_WrDat(0); 
				}
				Address3+=16;
			}
			if(i==8)
			{
				LCD_Set_Pos(Address4,7);
				for(j=0;j<6;j++)     
				{
					if(i+1==Value)
						LCD_WrDat(F6x8[10][j]); 
					else					
						LCD_WrDat(0); 
				} 
				Address4+=16;
			}
			if(i>8)
			{
				LCD_Set_Pos(Address4+3,7);
				for(j=0;j<6;j++)
				{				
					if(i+1==Value)
						LCD_WrDat(F6x8[10][j]); 
					else					
						LCD_WrDat(0);
				}					
				Address4+=16;
			}
		}
	}
	else
	{
		for(i=0;i<16;i++)
		{
			if(i<8)
			{
				LCD_Set_Pos(Address3,4);
				for(j=0;j<6;j++)     
					LCD_WrDat(0); 
				Address3+=16;
			}
			if(i==8)
			{
				LCD_Set_Pos(Address4,7);
				for(j=0;j<6;j++)     
					LCD_WrDat(0); 
				Address4+=16;
			}
			if(i>8)
			{
				LCD_Set_Pos(Address4+3,7);
				for(j=0;j<6;j++)     
					LCD_WrDat(0); 
				Address4+=16;
			}
		}
	}
}
void ShowStateValue(u8 Value)
{
	u8 Address1=45,i,j;
	u8 Value1[5]={'W','a','i','t',' '};
	u8 Value2[5]={'S','t','a','r','t'};
	if(Value==0)
	{
		for(i=0;i<5;i++)
		{
			LCD_Set_Pos(Address1,1);
			for(j=0;j<6;j++)     
				LCD_WrDat(F6x8[Value1[i]-32][j]); 
			Address1+=6;
		}
	}
	else
	{
		for(i=0;i<5;i++)
		{
			LCD_Set_Pos(Address1,1);
			for(j=0;j<6;j++)     
				LCD_WrDat(F6x8[Value2[i]-32][j]); 
			Address1+=6;
		}
	}
}
void ShowHoldTimeValue(u8 Value)
{
	u8 Address1=70,i,j;
	if(Value<10)
	{
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++)     
			LCD_WrDat(F6x8[16+Value][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(0);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(F6x8['s'-32][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(0);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(0);
	}
	if(Value>=10&&Value<100)
	{
		
		LCD_Set_Pos(Address1,0);
		i=Value/10;
		for(j=0;j<6;j++)     
			LCD_WrDat(F6x8[16+i][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		i=Value%10;
		for(j=0;j<6;j++)     
			LCD_WrDat(F6x8[16+i][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(0);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(F6x8['s'-32][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(0);
	}
	if(Value>=100)
	{
		LCD_Set_Pos(Address1,0);
		i=Value/100;
		for(j=0;j<6;j++)     
			LCD_WrDat(F6x8[16+i][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		i=(Value%100)/10;
		for(j=0;j<6;j++)     
			LCD_WrDat(F6x8[16+i][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		i=Value%10;
		for(j=0;j<6;j++)     
			LCD_WrDat(F6x8[16+i][j]);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(0);
		Address1+=6;
		LCD_Set_Pos(Address1,0);
		for(j=0;j<6;j++) 
			LCD_WrDat(F6x8['s'-32][j]);
	}
}
void LCD_ShowString(u8 x,u8 y,u8 *p)
{
	u8 i;
	while((*p<='~')&&(*p>=' '))
	{
		LCD_Set_Pos(x,y);
		for(i=0;i<6;i++) 
			LCD_WrDat(F6x8[*p-32][i]);
		x+=6;
		p++;
	}
}
void EC20Start(u8 num)
{
	if(num==0)
		LCD_ShowString(0,0,"4GStart:OK");
	else
		LCD_ShowString(0,0,"4GStart:FAIL");
}
void ShoWPowerHead(void)
{
	LCD_ShowString(0,2,"A_Power:");
	LCD_ShowString(90,2,"KWh");
}
void ShoWPowerData(u8 *Informations)
{
	u8 j;
	u8 Data[5];
	Data[0]=((Informations[15]-0x33)/16)+16;
	LCD_Set_Pos(50,2);
	for(j=0;j<6;j++) 
			LCD_WrDat(F6x8[Data[0]][j]);
	Data[1]=((Informations[15]-0x33)%16)+16;
	LCD_Set_Pos(56,2);
	for(j=0;j<6;j++) 
			LCD_WrDat(F6x8[Data[1]][j]);
	LCD_Set_Pos(62,2);
	for(j=0;j<6;j++) 
			LCD_WrDat(F6x8[14][j]);
	Data[3]=((Informations[14]-0x33)/16)+16;
	LCD_Set_Pos(68,2);
	for(j=0;j<6;j++) 
			LCD_WrDat(F6x8[Data[3]][j]);
	Data[4]=((Informations[14]-0x33)%16)+16;
	LCD_Set_Pos(74,2);
	for(j=0;j<6;j++) 
			LCD_WrDat(F6x8[Data[4]][j]);
}