#ifndef _eMMC_H
#define _eMMC_H
#include "sys.h"
#define EMMC_TIMEOUT      ((uint32_t)100000000)       //��ʱʱ��
#define EMMC_TRANSFER_OK     	((uint8_t)0x00)
#define EMMC_TRANSFER_BUSY   	((uint8_t)0x01)
#define EMMC_DMA_MODE     0	                        //1��DMAģʽ��0����ѯģʽ   

extern MMC_HandleTypeDef        EMMC_Handler;      //SD�����
extern HAL_MMC_CardInfoTypeDef  EMMCCardInfo;          //SD����Ϣ�ṹ��

HAL_StatusTypeDef eMMC_Init(void);
u8  eMMC_GetCardState(void);
u8 eMMC_ReadBlocks(uint8_t *buf, uint32_t BlockAdd, uint32_t cnt);
u8 eMMC_WriteBlocks(uint8_t *buf, uint32_t BlockAdd, uint32_t cnt);
#endif
