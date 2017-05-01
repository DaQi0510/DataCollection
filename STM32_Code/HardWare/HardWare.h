#ifndef __HardWare_H
#define __HardWare_H 	
#include "sys.h"

#define EC20_RESET        PBout(0)
#define EC20_WAKEUP_OUT   PBin(1)
#define EC20_WAKEUP_IN    PCout(4)
#define EC20_W_DISABLE    PCout(5)

#define RS485_TX_EN    PAout(1)

#define W25QXX_CS      PAout(4)

#define SET_LCD_DC GPIO_SetBits(GPIOC,GPIO_Pin_0)     //  PF0  片选
#define CLR_LCD_DC GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define SET_LCD_RST GPIO_SetBits(GPIOC,GPIO_Pin_1)    //  PF1  复位
#define CLR_LCD_RST GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define SET_LCD_SDA GPIO_SetBits(GPIOC,GPIO_Pin_2)     //  PF2  数据位
#define CLR_LCD_SDA GPIO_ResetBits(GPIOC,GPIO_Pin_2)

#define SET_LCD_SCL GPIO_SetBits(GPIOC,GPIO_Pin_3)     //  PF3  数据位
#define CLR_LCD_SCL GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#endif


