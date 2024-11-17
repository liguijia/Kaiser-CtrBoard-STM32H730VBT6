#ifndef _eMMC_H
#define _eMMC_H
#include "sys.h"
#define EMMC_TIMEOUT      ((uint32_t)100000000)       //超时时间
#define EMMC_TRANSFER_OK     	((uint8_t)0x00)
#define EMMC_TRANSFER_BUSY   	((uint8_t)0x01)
#define EMMC_DMA_MODE     0	                        //1：DMA模式，0：查询模式   

extern MMC_HandleTypeDef        EMMC_Handler;      //SD卡句柄
extern HAL_MMC_CardInfoTypeDef  EMMCCardInfo;          //SD卡信息结构体

HAL_StatusTypeDef eMMC_Init(void);
u8  eMMC_GetCardState(void);
u8 eMMC_ReadBlocks(uint8_t *buf, uint32_t BlockAdd, uint32_t cnt);
u8 eMMC_WriteBlocks(uint8_t *buf, uint32_t BlockAdd, uint32_t cnt);
#endif
