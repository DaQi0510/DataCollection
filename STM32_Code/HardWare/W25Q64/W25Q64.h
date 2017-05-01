#ifndef __W25Q64_H
#define __W25Q64_H 	

#include "stm32f10x.h"
#include "misc.h"
#include "delay.h"
#include "HardWare.h"
#include "stm32f10x_spi.h"

#define W25X_WriteEnable		  0x06 
#define W25X_WriteDisable		  0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			    0x03 
#define W25X_PageProgram		  0x02 
#define W25X_BlockErase			  0xD8 
#define W25X_SectorErase		  0x20 
#define W25X_ChipErase			  0xC7 

void W25Q64_Init(void);
u16 W25QXX_ReadID(void);
u8 SPI1_ReadWriteByte(u8 TxData);

u8	 W25QXX_ReadSR(void);        		//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷ 
void W25QXX_Write_SR(u8 sr);  			//Ð´×´Ì¬¼Ä´æÆ÷
void W25QXX_Write_Enable(void);  		//Ð´Ê¹ÄÜ 
void W25QXX_Write_Disable(void);		//Ð´±£»¤
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //¶ÁÈ¡flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//Ð´Èëflash
void W25QXX_Erase_Chip(void);    	  	//ÕûÆ¬²Á³ý
void W25QXX_Erase_Sector(u32 Dst_Addr);	//ÉÈÇø²Á³ý
void W25QXX_Wait_Busy(void);           	//µÈ´ý¿ÕÏÐ
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

#endif



