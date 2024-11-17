#include "Sram.h"
#define SRAM_BANK_ADDR                 ((uint32_t)0x68000000)
SRAM_HandleTypeDef hsram1;

void FSMC_SRAM_Init(void)
{
	FMC_NORSRAM_TimingTypeDef Timing;

	/** Perform the SRAM1 memory initialization sequence
	*/
	hsram1.Instance = FMC_NORSRAM_DEVICE;
	hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
	/* Timing */	
	Timing.AddressSetupTime      = 2;
	Timing.AddressHoldTime       = 1;
	Timing.DataSetupTime         = 2;
	Timing.BusTurnAroundDuration = 1;
	Timing.CLKDivision           = 2;
	Timing.DataLatency           = 2;
	Timing.AccessMode            = FMC_ACCESS_MODE_A;
	/* hsram1.Init */
	hsram1.Init.NSBank = FMC_NORSRAM_BANK3;
	hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
	hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
	hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
	hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
	hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
	hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
	hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
	hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
//	hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
	/* Timing */
//	Timing.AddressSetupTime = 0x00;
//	Timing.AddressHoldTime = 0x00;
//	Timing.DataSetupTime = 0x0C;
//	Timing.BusTurnAroundDuration = 0x00;
//	Timing.CLKDivision =0x00;
//	Timing.DataLatency = 0x00;
//	Timing.AccessMode = FMC_ACCESS_MODE_A;
	HAL_SRAM_Init(&hsram1, &Timing, &Timing);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_FMC_CLK_ENABLE();
	
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9
													|GPIO_PIN_11 |GPIO_PIN_14|GPIO_PIN_7|GPIO_PIN_10
													|GPIO_PIN_12 |GPIO_PIN_15|GPIO_PIN_13;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_1 
													|GPIO_PIN_4|GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_14 
													|GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_12|GPIO_PIN_13 
													|GPIO_PIN_11;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_2 
													|GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_4;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
													|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_15 
													|GPIO_PIN_13|GPIO_PIN_14;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

void FSMC_SRAM_WriteBuffer(u16* buf,u32 WriteAddr,u32 n)
{
	HAL_SRAM_Write_16b(&hsram1, (uint32_t *)(SRAM_BANK_ADDR + WriteAddr*2), buf, n);
}

void FSMC_SRAM_ReadBuffer(u16* buf,u32 WriteAddr,u32 n)
{
	HAL_SRAM_Read_16b(&hsram1, (uint32_t *)(SRAM_BANK_ADDR + WriteAddr*2), buf, n);
}
