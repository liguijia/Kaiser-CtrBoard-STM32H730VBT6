#include "eMMC.h"
#include "string.h"
#include "sys.h"
#include "delay.h"
MMC_HandleTypeDef        EMMC_Handler;             //SD�����
HAL_MMC_CardInfoTypeDef  EMMCCardInfo;                 //SD����Ϣ�ṹ��
__align(4) u8 SDIO_DATA_BUFFER[512];
HAL_StatusTypeDef eMMC_Init(void)
{
    HAL_StatusTypeDef EMMC_Status;
    //��ʼ��ʱ��ʱ�Ӳ��ܴ���400KHZ 
    EMMC_Handler.Instance=SDMMC1;
    EMMC_Handler.Init.ClockEdge=SDMMC_CLOCK_EDGE_RISING;              //������     
    EMMC_Handler.Init.ClockBypass=SDMMC_CLOCK_BYPASS_DISABLE;         //��ʹ��bypassģʽ��ֱ����HCLK���з�Ƶ�õ�SDIO_CK
    EMMC_Handler.Init.ClockPowerSave=SDMMC_CLOCK_POWER_SAVE_DISABLE;  //����ʱ���ر�ʱ�ӵ�Դ
    EMMC_Handler.Init.BusWide=SDMMC_BUS_WIDE_1B;                      //1λ������
    EMMC_Handler.Init.HardwareFlowControl=SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;//�ر�Ӳ������
    EMMC_Handler.Init.ClockDiv=SDMMC_TRANSFER_CLK_DIV;                //SD����ʱ��Ƶ�����25MHZ
    EMMC_Status = HAL_MMC_Init(&EMMC_Handler);
    if(EMMC_Status!=HAL_OK) 
		return EMMC_Status;
	EMMC_Status = HAL_MMC_ConfigWideBusOperation(&EMMC_Handler, SDMMC_BUS_WIDE_4B);  //ʹ�ܿ�����ģʽ
    if(EMMC_Status!=HAL_OK) 
		return EMMC_Status;
    return HAL_OK;
}

void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_SDMMC1_CLK_ENABLE();  //ʹ��SDMMC1ʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();   //ʹ��GPIODʱ��
    
    //PC8,9,10,11,12
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //���츴��
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //���� GPIO_PULLDOWN
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF12_SDIO;  //����ΪSDIO
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //��ʼ��
    
    //PD2
    GPIO_Initure.Pin=GPIO_PIN_2;            
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //��ʼ��
}
u8  eMMC_GetCardState(void)
{
	return((HAL_MMC_GetCardState(&EMMC_Handler)==HAL_MMC_CARD_TRANSFER )?EMMC_TRANSFER_OK:EMMC_TRANSFER_BUSY);
}
//buf:������
//BlockAdd�����ַ
//NumberOfBlocks:����� SDIO_DATA_BUFFER
u8 eMMC_ReadBlocks(uint8_t *buf, uint32_t BlockAdd, uint32_t cnt)
{
	u8 eMMC_Status = HAL_OK;
	long long lBlockAdd = BlockAdd;
	u32 timeout=EMMC_TIMEOUT;
	INTX_DISABLE();
	eMMC_Status = HAL_MMC_ReadBlocks(&EMMC_Handler,(uint8_t *)buf,lBlockAdd,cnt,EMMC_TIMEOUT);
	while(eMMC_GetCardState()!= EMMC_TRANSFER_OK)
	{
		if(timeout-- == 0)
		{	
			eMMC_Status=EMMC_TRANSFER_BUSY;
		}
	}	
	INTX_ENABLE();
	return eMMC_Status;
}

u8 eMMC_WriteBlocks(uint8_t *buf, uint32_t BlockAdd, uint32_t cnt)
{
	u8 eMMC_Status = HAL_OK;
	long long lBlockAdd = BlockAdd;
	u32 timeout=EMMC_TIMEOUT;
	INTX_DISABLE();
	eMMC_Status = HAL_MMC_WriteBlocks(&EMMC_Handler,(uint8_t*)buf,lBlockAdd,cnt,EMMC_TIMEOUT);
	while(eMMC_GetCardState()!= EMMC_TRANSFER_OK)
	{
		if(timeout-- == 0)
		{	
			eMMC_Status=EMMC_TRANSFER_BUSY;
		}
	}
	INTX_ENABLE();
	return eMMC_Status;
}
