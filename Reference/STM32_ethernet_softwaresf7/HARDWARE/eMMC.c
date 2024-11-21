#include "eMMC.h"
#include "string.h"
#include "sys.h"
#include "delay.h"
MMC_HandleTypeDef        EMMC_Handler;             //SD卡句柄
HAL_MMC_CardInfoTypeDef  EMMCCardInfo;                 //SD卡信息结构体
__align(4) u8 SDIO_DATA_BUFFER[512];
HAL_StatusTypeDef eMMC_Init(void)
{
    HAL_StatusTypeDef EMMC_Status;
    //初始化时的时钟不能大于400KHZ 
    EMMC_Handler.Instance=SDMMC1;
    EMMC_Handler.Init.ClockEdge=SDMMC_CLOCK_EDGE_RISING;              //上升沿     
    EMMC_Handler.Init.ClockBypass=SDMMC_CLOCK_BYPASS_DISABLE;         //不使用bypass模式，直接用HCLK进行分频得到SDIO_CK
    EMMC_Handler.Init.ClockPowerSave=SDMMC_CLOCK_POWER_SAVE_DISABLE;  //空闲时不关闭时钟电源
    EMMC_Handler.Init.BusWide=SDMMC_BUS_WIDE_1B;                      //1位数据线
    EMMC_Handler.Init.HardwareFlowControl=SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;//关闭硬件流控
    EMMC_Handler.Init.ClockDiv=SDMMC_TRANSFER_CLK_DIV;                //SD传输时钟频率最大25MHZ
    EMMC_Status = HAL_MMC_Init(&EMMC_Handler);
    if(EMMC_Status!=HAL_OK) 
		return EMMC_Status;
	EMMC_Status = HAL_MMC_ConfigWideBusOperation(&EMMC_Handler, SDMMC_BUS_WIDE_4B);  //使能宽总线模式
    if(EMMC_Status!=HAL_OK) 
		return EMMC_Status;
    return HAL_OK;
}

void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_SDMMC1_CLK_ENABLE();  //使能SDMMC1时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    
    //PC8,9,10,11,12
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //推挽复用
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //上拉 GPIO_PULLDOWN
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF12_SDIO;  //复用为SDIO
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //初始化
    
    //PD2
    GPIO_Initure.Pin=GPIO_PIN_2;            
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
}
u8  eMMC_GetCardState(void)
{
	return((HAL_MMC_GetCardState(&EMMC_Handler)==HAL_MMC_CARD_TRANSFER )?EMMC_TRANSFER_OK:EMMC_TRANSFER_BUSY);
}
//buf:缓冲区
//BlockAdd：块地址
//NumberOfBlocks:块个数 SDIO_DATA_BUFFER
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
