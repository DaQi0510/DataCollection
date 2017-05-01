#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"
#include "delay.h"
#include "HardWare.h"

 void LCD_Init(void);
 void LCD_P6x8Str(u8 x,u8 y,u8 ch[]);
 void LCD_ShowNum(u8 x,u8 y, unsigned int num);
 void LCD_Show_M(u8 y,unsigned long int num);
 void LCD_Display(void);
 void LCD_ShowADNum(u8 Value);
 void LCD_ShowSqua_Fre(u16 Value);
 void LCD_ShowSine_Fre(u16 Value);
 void LCD_ShowSine_Amp(u16 Value);
 void LCD_ShowSwitch_Flag(u8 Value);
 void Display(void);
 void ShowChannel(u8 Value);
 void ShowStateValue(u8 Value);
 void ShowHoldTimeValue(u8 Value);
 
#endif

