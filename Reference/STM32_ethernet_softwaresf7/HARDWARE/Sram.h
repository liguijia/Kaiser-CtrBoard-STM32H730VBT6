#ifndef __SRAM_H
#define __SRAM_H															    
#include "sys.h" 
extern SRAM_HandleTypeDef hsram1;
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u16* buf,u32 WriteAddr,u32 n);

void FSMC_SRAM_ReadBuffer(u16* buf,u32 WriteAddr,u32 n);
#endif
