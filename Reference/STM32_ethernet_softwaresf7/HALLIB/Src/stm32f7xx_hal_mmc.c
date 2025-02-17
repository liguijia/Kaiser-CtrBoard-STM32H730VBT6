/**
  ******************************************************************************
  * @file    stm32f7xx_hal_mmc.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    14-April-2017
  * @brief   MMC card HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Secure Digital (MMC) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions 
  *           + MMC card Control functions
  *         
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    This driver implements a high level communication layer for read and write from/to 
    this memory. The needed STM32 hardware resources (SDMMC and GPIO) are performed by 
    the user in HAL_MMC_MspInit() function (MSP layer).                             
    Basically, the MSP layer configuration should be the same as we provide in the 
    examples.
    You can easily tailor this configuration according to hardware resources.

  [..]
    This driver is a generic layered driver for SDMMC memories which uses the HAL 
    SDMMC driver functions to interface with MMC and eMMC cards devices. 
    It is used as follows:
 
    (#)Initialize the SDMMC low level resources by implement the HAL_MMC_MspInit() API:
        (##) Enable the SDMMC interface clock using __HAL_RCC_SDMMC_CLK_ENABLE(); 
        (##) SDMMC pins configuration for MMC card
            (+++) Enable the clock for the SDMMC GPIOs using the functions __HAL_RCC_GPIOx_CLK_ENABLE();   
            (+++) Configure these SDMMC pins as alternate function pull-up using HAL_GPIO_Init()
                  and according to your pin assignment;
        (##) DMA Configuration if you need to use DMA process (HAL_MMC_ReadBlocks_DMA()
             and HAL_MMC_WriteBlocks_DMA() APIs).
            (+++) Enable the DMAx interface clock using __HAL_RCC_DMAx_CLK_ENABLE(); 
            (+++) Configure the DMA using the function HAL_DMA_Init() with predeclared and filled. 
        (##) NVIC configuration if you need to use interrupt process when using DMA transfer.
            (+++) Configure the SDMMC and DMA interrupt priorities using functions
                  HAL_NVIC_SetPriority(); DMA priority is superior to SDMMC's priority
            (+++) Enable the NVIC DMA and SDMMC IRQs using function HAL_NVIC_EnableIRQ()
            (+++) SDMMC interrupts are managed using the macros __HAL_MMC_ENABLE_IT() 
                  and __HAL_MMC_DISABLE_IT() inside the communication process.
            (+++) SDMMC interrupts pending bits are managed using the macros __HAL_MMC_GET_IT()
                  and __HAL_MMC_CLEAR_IT()
        (##) NVIC configuration if you need to use interrupt process (HAL_MMC_ReadBlocks_IT()
             and HAL_MMC_WriteBlocks_IT() APIs).
            (+++) Configure the SDMMC interrupt priorities using function
                  HAL_NVIC_SetPriority();
            (+++) Enable the NVIC SDMMC IRQs using function HAL_NVIC_EnableIRQ()
            (+++) SDMMC interrupts are managed using the macros __HAL_MMC_ENABLE_IT() 
                  and __HAL_MMC_DISABLE_IT() inside the communication process.
            (+++) SDMMC interrupts pending bits are managed using the macros __HAL_MMC_GET_IT()
                  and __HAL_MMC_CLEAR_IT()
    (#) At this stage, you can perform MMC read/write/erase operations after MMC card initialization  

         
  *** MMC Card Initialization and configuration ***
  ================================================    
  [..]
    To initialize the MMC Card, use the HAL_MMC_Init() function. It Initializes 
    SDMMC IP (STM32 side) and the MMC Card, and put it into StandBy State (Ready for data transfer). 
    This function provide the following operations:

    (#) Initialize the SDMMC peripheral interface with defaullt configuration.
        The initialization process is done at 400KHz. You can change or adapt 
        this frequency by adjusting the "ClockDiv" field. 
        The MMC Card frequency (SDMMC_CK) is computed as follows:
  
           SDMMC_CK = SDMMCCLK / (ClockDiv + 2)
  
        In initialization mode and according to the MMC Card standard, 
        make sure that the SDMMC_CK frequency doesn't exceed 400KHz.

        This phase of initialization is done through SDMMC_Init() and 
        SDMMC_PowerState_ON() SDMMC low level APIs.

    (#) Initialize the MMC card. The API used is HAL_MMC_InitCard().
        This phase allows the card initialization and identification 
        and check the MMC Card type (Standard Capacity or High Capacity)
        The initialization flow is compatible with MMC standard.

        This API (HAL_MMC_InitCard()) could be used also to reinitialize the card in case 
        of plug-off plug-in.
  
    (#) Configure the MMC Card Data transfer frequency. By Default, the card transfer 
        frequency is set to 24MHz. You can change or adapt this frequency by adjusting 
        the "ClockDiv" field.
        In transfer mode and according to the MMC Card standard, make sure that the 
        SDMMC_CK frequency doesn't exceed 25MHz and 50MHz in High-speed mode switch.
        To be able to use a frequency higher than 24MHz, you should use the SDMMC 
        peripheral in bypass mode. Refer to the corresponding reference manual 
        for more details.
  
    (#) Select the corresponding MMC Card according to the address read with the step 2.
    
    (#) Configure the MMC Card in wide bus mode: 4-bits data.
  
  *** MMC Card Read operation ***
  ==============================
  [..] 
    (+) You can read from MMC card in polling mode by using function HAL_MMC_ReadBlocks(). 
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.

    (+) You can read from MMC card in DMA mode by using function HAL_MMC_ReadBlocks_DMA().
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the DMA transfer process through the MMC Rx interrupt event.

    (+) You can read from MMC card in Interrupt mode by using function HAL_MMC_ReadBlocks_IT().
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the IT transfer process through the MMC Rx interrupt event.
  
  *** MMC Card Write operation ***
  =============================== 
  [..] 
    (+) You can write to MMC card in polling mode by using function HAL_MMC_WriteBlocks(). 
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.

    (+) You can write to MMC card in DMA mode by using function HAL_MMC_WriteBlocks_DMA().
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the DMA transfer process through the MMC Tx interrupt event.  

    (+) You can write to MMC card in Interrupt mode by using function HAL_MMC_WriteBlocks_IT().
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the IT transfer process through the MMC Tx interrupt event.

  *** MMC card information ***
  =========================== 
  [..]
    (+) To get MMC card information, you can use the function HAL_MMC_GetCardInfo().
        It returns useful information about the MMC card such as block size, card type,
        block number ...

  *** MMC card CSD register ***
  ============================
  [..]
    (+) The HAL_MMC_GetCardCSD() API allows to get the parameters of the CSD register.
        Some of the CSD parameters are useful for card initialization and identification.

  *** MMC card CID register ***
  ============================
  [..]
    (+) The HAL_MMC_GetCardCID() API allows to get the parameters of the CID register.
        Some of the CID parameters are useful for card initialization and identification.

  *** MMC HAL driver macros list ***
  ==================================
  [..]
    Below the list of most used macros in MMC HAL driver.
       
    (+) __HAL_MMC_ENABLE : Enable the MMC device
    (+) __HAL_MMC_DISABLE : Disable the MMC device
    (+) __HAL_MMC_DMA_ENABLE: Enable the SDMMC DMA transfer
    (+) __HAL_MMC_DMA_DISABLE: Disable the SDMMC DMA transfer
    (+) __HAL_MMC_ENABLE_IT: Enable the MMC device interrupt
    (+) __HAL_MMC_DISABLE_IT: Disable the MMC device interrupt
    (+) __HAL_MMC_GET_FLAG:Check whether the specified MMC flag is set or not
    (+) __HAL_MMC_CLEAR_FLAG: Clear the MMC's pending flags

  [..]
    (@) You can refer to the MMC HAL driver header file for more useful macros 
      
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup STM32F7xx_HAL_Driver
  * @{
  */

/** @defgroup MMC MMC
  * @brief MMC HAL module driver
  * @{
  */

#ifdef HAL_MMC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup MMC_Private_Defines
  * @{
  */
    
/**
  * @}
  */
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup MMC_Private_Functions MMC Private Functions
  * @{
  */
static uint32_t MMC_InitCard(MMC_HandleTypeDef *hmmc);
static uint32_t MMC_PowerON(MMC_HandleTypeDef *hmmc);
static uint32_t MMC_SendStatus(MMC_HandleTypeDef *hmmc, uint32_t *pCardStatus);
static HAL_StatusTypeDef MMC_PowerOFF(MMC_HandleTypeDef *hmmc);
static HAL_StatusTypeDef MMC_Write_IT(MMC_HandleTypeDef *hmmc);
static HAL_StatusTypeDef MMC_Read_IT(MMC_HandleTypeDef *hmmc);
static void MMC_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void MMC_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void MMC_DMAError(DMA_HandleTypeDef *hdma);
static void MMC_DMATxAbort(DMA_HandleTypeDef *hdma);
static void MMC_DMARxAbort(DMA_HandleTypeDef *hdma);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup MMC_Exported_Functions
  * @{
  */

/** @addtogroup MMC_Exported_Functions_Group1
 *  @brief   Initialization and de-initialization functions 
 *
@verbatim    
  ==============================================================================
          ##### Initialization and de-initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to initialize/de-initialize the MMC
    card device to be ready for use.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the MMC according to the specified parameters in the 
            MMC_HandleTypeDef and create the associated handle.
  * @param  hmmc: Pointer to the MMC handle  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Init(MMC_HandleTypeDef *hmmc)
{
  /* Check the MMC handle allocation */
  if(hmmc == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_SDMMC_ALL_INSTANCE(hmmc->Instance));
  assert_param(IS_SDMMC_CLOCK_EDGE(hmmc->Init.ClockEdge));
  assert_param(IS_SDMMC_CLOCK_BYPASS(hmmc->Init.ClockBypass));
  assert_param(IS_SDMMC_CLOCK_POWER_SAVE(hmmc->Init.ClockPowerSave));
  assert_param(IS_SDMMC_BUS_WIDE(hmmc->Init.BusWide));
  assert_param(IS_SDMMC_HARDWARE_FLOW_CONTROL(hmmc->Init.HardwareFlowControl));
  assert_param(IS_SDMMC_CLKDIV(hmmc->Init.ClockDiv));

  if(hmmc->State == HAL_MMC_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hmmc->Lock = HAL_UNLOCKED;
    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_MMC_MspInit(hmmc);
  }

  hmmc->State = HAL_MMC_STATE_BUSY;

  /* Initialize the Card parameters */
  HAL_MMC_InitCard(hmmc);

  /* Initialize the error code */
  hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
  
  /* Initialize the MMC operation */
  hmmc->Context = MMC_CONTEXT_NONE;
                                                                                     
  /* Initialize the MMC state */
  hmmc->State = HAL_MMC_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  Initializes the MMC Card.
  * @param  hmmc: Pointer to MMC handle
  * @note   This function initializes the MMC card. It could be used when a card 
            re-initialization is needed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_InitCard(MMC_HandleTypeDef *hmmc)
{
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  MMC_InitTypeDef Init;
  
  /* Default SDMMC peripheral configuration for MMC card initialization */
  Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
  Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  Init.BusWide             = SDMMC_BUS_WIDE_1B;
  Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  Init.ClockDiv            = SDMMC_INIT_CLK_DIV;

  /* Initialize SDMMC peripheral interface with default configuration */
  SDMMC_Init(hmmc->Instance, Init);

  /* Disable SDMMC Clock */
  __HAL_MMC_DISABLE(hmmc); 
  
  /* Set Power State to ON */
  SDMMC_PowerState_ON(hmmc->Instance);
  
  /* Enable MMC Clock */
  __HAL_MMC_ENABLE(hmmc);
  
  /* Required power up waiting time before starting the SD initialization sequence */
  HAL_Delay(2);
  
  /* Identify card operating voltage */
  errorstate = MMC_PowerON(hmmc);
  if(errorstate != HAL_MMC_ERROR_NONE)
  {
    hmmc->State = HAL_MMC_STATE_READY;
    hmmc->ErrorCode |= errorstate;
    return HAL_ERROR;
  }

  /* Card initialization */
  errorstate = MMC_InitCard(hmmc);
  if(errorstate != HAL_MMC_ERROR_NONE)
  {
    hmmc->State = HAL_MMC_STATE_READY;
    hmmc->ErrorCode |= errorstate;
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  De-Initializes the MMC card.
  * @param  hmmc: Pointer to MMC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_DeInit(MMC_HandleTypeDef *hmmc)
{
  /* Check the MMC handle allocation */
  if(hmmc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_SDMMC_ALL_INSTANCE(hmmc->Instance));

  hmmc->State = HAL_MMC_STATE_BUSY;
  
  /* Set MMC power state to off */ 
  MMC_PowerOFF(hmmc);
  
  /* De-Initialize the MSP layer */
  HAL_MMC_MspDeInit(hmmc);
  
  hmmc->ErrorCode = HAL_MMC_ERROR_NONE;
  hmmc->State = HAL_MMC_STATE_RESET;
  
  return HAL_OK;
}


/**
  * @brief  Initializes the MMC MSP.
  * @param  hmmc: Pointer to MMC handle
  * @retval None
  */
__weak void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);
 
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MMC_MspInit could be implemented in the user file
   */
}

/**
  * @brief  De-Initialize MMC MSP.
  * @param  hmmc: Pointer to MMC handle
  * @retval None
  */
__weak void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);
 
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MMC_MspDeInit could be implemented in the user file
   */
}

/**
  * @}
  */

/** @addtogroup MMC_Exported_Functions_Group2
 *  @brief   Data transfer functions 
 *
@verbatim   
  ==============================================================================
                        ##### IO operation functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to manage the data 
    transfer from/to MMC card.

@endverbatim
  * @{
  */

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read 
  * @param  NumberOfBlocks: Number of MMC blocks to read
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
  SDMMC_DataInitTypeDef config;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  uint32_t tickstart = HAL_GetTick();
  uint32_t count = 0, *tempbuff = (uint32_t *)pData;
  
  if(NULL == pData)
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
    return HAL_ERROR;
  }
 
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    
//  if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
	if((BlockAdd + NumberOfBlocks) > 0x00E88000)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_BUSY;
    
    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0;  
    /* Set Block Size for Card */
    errorstate = SDMMC_CmdBlockLength(hmmc->Instance, BLOCKSIZE);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);      
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Configure the MMC DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = NumberOfBlocks * BLOCKSIZE;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_SDMMC;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_ENABLE;
    SDMMC_ConfigData(hmmc->Instance, &config);
    
    /* Read block(s) in polling mode */
    if(NumberOfBlocks > 1)
    {
      hmmc->Context = MMC_CONTEXT_READ_MULTIPLE_BLOCK;
      
      /* Read Multi Block command */ 
      errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, (uint32_t)BlockAdd);
    }
    else
    {
      hmmc->Context = MMC_CONTEXT_READ_SINGLE_BLOCK;
      
      /* Read Single Block command */
      errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, (uint32_t)BlockAdd);
    }
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
      
    /* Poll on SDMMC flags */
    while(!__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_RXOVERR | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DATAEND))
    {
      if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_RXFIFOHF))
      {
        /* Read data from SDMMC Rx FIFO */
        for(count = 0U; count < 8U; count++)
        {
          *(tempbuff + count) = SDMMC_ReadFIFO(hmmc->Instance);
        }
        tempbuff += 8U;
      }
      
      if((Timeout == 0U)||((HAL_GetTick()-tickstart) >=  Timeout))
      {
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
        hmmc->State= HAL_MMC_STATE_READY;
        return HAL_TIMEOUT;
      }
    }
    
    /* Send stop transmission command in case of multiblock read */
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_DATAEND) && (NumberOfBlocks > 1U))
    {
      /* Send stop transmission command */
      errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
      if(errorstate != HAL_MMC_ERROR_NONE)
      {
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
        hmmc->ErrorCode |= errorstate;
        hmmc->State = HAL_MMC_STATE_READY;
        return HAL_ERROR;
      }
    }
    
    /* Get error state */
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_DTIMEOUT))
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_DCRCFAIL))
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_RXOVERR))
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_RX_OVERRUN;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Empty FIFO if there is still any data */
    while ((__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_RXDAVL)))
    {
      *tempbuff = SDMMC_ReadFIFO(hmmc->Instance);
      tempbuff++;
      
      if((Timeout == 0U)||((HAL_GetTick()-tickstart) >=  Timeout))
      {
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);        
        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
        hmmc->State= HAL_MMC_STATE_READY;
        return HAL_ERROR;
      }
    }
    
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
    
    hmmc->State = HAL_MMC_STATE_READY;
    
    return HAL_OK;
  }
  else
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;
    return HAL_ERROR;
  }
}

/**
  * @brief  Allows to write block(s) to a specified address in a card. The Data
  *         transfer is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written  
  * @param  NumberOfBlocks: Number of MMC blocks to write
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
  SDMMC_DataInitTypeDef config;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  uint32_t tickstart = HAL_GetTick();
  uint32_t count = 0;
  uint32_t *tempbuff = (uint32_t *)pData;
  
  if(NULL == pData)
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
    return HAL_ERROR;
  }

  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    
    if((BlockAdd + NumberOfBlocks) > 0x00E88000)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_BUSY;
    
    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0;
    /* Set Block Size for Card */ 
    errorstate = SDMMC_CmdBlockLength(hmmc->Instance, BLOCKSIZE);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);  
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)
    {
      hmmc->Context = MMC_CONTEXT_WRITE_MULTIPLE_BLOCK;
      
      /* Write Multi Block command */ 
      errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, (uint32_t)BlockAdd);
    }
    else
    {
      hmmc->Context = MMC_CONTEXT_WRITE_SINGLE_BLOCK;
      
      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, (uint32_t)BlockAdd);
    }
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);  
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Configure the MMC DPSM (Data Path State Machine) */ 
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = NumberOfBlocks * BLOCKSIZE;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_CARD;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_ENABLE;
    SDMMC_ConfigData(hmmc->Instance, &config);
    
    /* Write block(s) in polling mode */
    while(!__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_TXUNDERR | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DATAEND))
    {
      if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_TXFIFOHE))
      {
        /* Write data to SDMMC Tx FIFO */
        for(count = 0U; count < 8U; count++)
        {
          SDMMC_WriteFIFO(hmmc->Instance, (tempbuff + count));
        }
        tempbuff += 8U;
      }
      
      if((Timeout == 0U)||((HAL_GetTick()-tickstart) >=  Timeout))
      {
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);  
        hmmc->ErrorCode |= errorstate;
        hmmc->State = HAL_MMC_STATE_READY;
        return HAL_TIMEOUT;
      }
    }
    
    /* Send stop transmission command in case of multiblock write */
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_DATAEND) && (NumberOfBlocks > 1U))
    { 
      /* Send stop transmission command */
      errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
      if(errorstate != HAL_MMC_ERROR_NONE)
      {
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);  
        hmmc->ErrorCode |= errorstate;
        hmmc->State = HAL_MMC_STATE_READY;
        return HAL_ERROR;
      }
    }
    
    /* Get error state */
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_DTIMEOUT))
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_DCRCFAIL))
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL;      
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_FLAG_TXUNDERR))
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_TX_UNDERRUN;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
    
    hmmc->State = HAL_MMC_STATE_READY;
    
    return HAL_OK;
  }
  else
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;
    return HAL_ERROR;
  }
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed in interrupt mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @note   You could also check the IT transfer process through the MMC Rx 
  *         interrupt event.
  * @param  hmmc: Pointer to MMC handle                 
  * @param  pData: Pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read 
  * @param  NumberOfBlocks: Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
  SDMMC_DataInitTypeDef config;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  if(NULL == pData)
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
    return HAL_ERROR;
  }
  
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    
    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_BUSY;
    
    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;
    
    hmmc->pRxBuffPtr = (uint32_t *)pData;
    hmmc->RxXferSize = BLOCKSIZE * NumberOfBlocks;
    
    __HAL_MMC_ENABLE_IT(hmmc, (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_RXOVERR | SDMMC_IT_DATAEND | SDMMC_FLAG_RXFIFOHF));
    
    /* Check the Card capacity in term of Logical number of blocks */
    if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
    {
      BlockAdd *= 512;
    }
    
    /* Configure the MMC DPSM (Data Path State Machine) */ 
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = BLOCKSIZE * NumberOfBlocks;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_SDMMC;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_ENABLE;
    SDMMC_ConfigData(hmmc->Instance, &config);
    
    /* Set Block Size for Card */ 
    errorstate = SDMMC_CmdBlockLength(hmmc->Instance, BLOCKSIZE);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }

    /* Read Blocks in IT mode */
    if(NumberOfBlocks > 1U)
    {
      hmmc->Context = (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_IT);
      
      /* Read Multi Block command */
      errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, BlockAdd);
    }
    else
    {
      hmmc->Context = (MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_IT);
      
      /* Read Single Block command */
      errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, BlockAdd);
    }
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Writes block(s) to a specified address in a card. The Data transfer 
  *         is managed in interrupt mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @note   You could also check the IT transfer process through the MMC Tx 
  *         interrupt event. 
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written    
  * @param  NumberOfBlocks: Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
  SDMMC_DataInitTypeDef config;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  if(NULL == pData)
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
    return HAL_ERROR;
  }
  
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    
    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_BUSY;
    
    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;
    
    hmmc->pTxBuffPtr = (uint32_t *)pData;
    hmmc->TxXferSize = BLOCKSIZE * NumberOfBlocks;
    
    /* Enable transfer interrupts */
    __HAL_MMC_ENABLE_IT(hmmc, (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_TXUNDERR | SDMMC_IT_DATAEND | SDMMC_FLAG_TXFIFOHE)); 
    
    /* Check the Card capacity in term of Logical number of blocks */
    if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
    {
      BlockAdd *= 512;
    }
    
    /* Set Block Size for Card */ 
    errorstate = SDMMC_CmdBlockLength(hmmc->Instance, BLOCKSIZE);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)
    {
      hmmc->Context = (MMC_CONTEXT_WRITE_MULTIPLE_BLOCK| MMC_CONTEXT_IT);
      
      /* Write Multi Block command */ 
      errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, BlockAdd);
    }
    else
    {
      hmmc->Context = (MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_IT);
      
      /* Write Single Block command */ 
      errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, BlockAdd);
    }
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Configure the MMC DPSM (Data Path State Machine) */ 
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = BLOCKSIZE * NumberOfBlocks;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_CARD;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_ENABLE;
    SDMMC_ConfigData(hmmc->Instance, &config);
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed by DMA mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @note   You could also check the DMA transfer process through the MMC Rx 
  *         interrupt event.
  * @param  hmmc: Pointer MMC handle                 
  * @param  pData: Pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read  
  * @param  NumberOfBlocks: Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_DMA(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
  SDMMC_DataInitTypeDef config;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  if(NULL == pData)
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
    return HAL_ERROR;
  }
  
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    
    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_BUSY;
    
    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;
    
    __HAL_MMC_ENABLE_IT(hmmc, (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_RXOVERR | SDMMC_IT_DATAEND));
    
    /* Set the DMA transfer complete callback */
    hmmc->hdmarx->XferCpltCallback = MMC_DMAReceiveCplt;
    
    /* Set the DMA error callback */
    hmmc->hdmarx->XferErrorCallback = MMC_DMAError;
    
    /* Set the DMA Abort callback */
    hmmc->hdmarx->XferAbortCallback = NULL;
    
    /* Enable the DMA Channel */
    HAL_DMA_Start_IT(hmmc->hdmarx, (uint32_t)&hmmc->Instance->FIFO, (uint32_t)pData, (uint32_t)(BLOCKSIZE * NumberOfBlocks)/4);
    
    /* Enable MMC DMA transfer */
    __HAL_MMC_DMA_ENABLE(hmmc);
    
    /* Check the Card capacity in term of Logical number of blocks */
    if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
    {
      BlockAdd *= 512;
    }
    
    /* Configure the MMC DPSM (Data Path State Machine) */ 
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = BLOCKSIZE * NumberOfBlocks;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_SDMMC;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_ENABLE;
    SDMMC_ConfigData(hmmc->Instance, &config);

    /* Set Block Size for Card */ 
    errorstate = SDMMC_CmdBlockLength(hmmc->Instance, BLOCKSIZE);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
        
    /* Read Blocks in DMA mode */
    if(NumberOfBlocks > 1U)
    {
      hmmc->Context = (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_DMA);
      
      /* Read Multi Block command */ 
      errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, BlockAdd);
    }
    else
    {
      hmmc->Context = (MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_DMA);
      
      /* Read Single Block command */ 
      errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, BlockAdd);
    }
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Writes block(s) to a specified address in a card. The Data transfer 
  *         is managed by DMA mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @note   You could also check the DMA transfer process through the MMC Tx 
  *         interrupt event.
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written  
  * @param  NumberOfBlocks: Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks_DMA(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
  SDMMC_DataInitTypeDef config;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  if(NULL == pData)
  {
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
    return HAL_ERROR;
  }
  
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    
    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_BUSY;
    
    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;
    
    /* Enable MMC Error interrupts */
    __HAL_MMC_ENABLE_IT(hmmc, (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_TXUNDERR));    
    
    /* Set the DMA transfer complete callback */
    hmmc->hdmatx->XferCpltCallback = MMC_DMATransmitCplt;
    
    /* Set the DMA error callback */
    hmmc->hdmatx->XferErrorCallback = MMC_DMAError;
    
    /* Set the DMA Abort callback */
    hmmc->hdmatx->XferAbortCallback = NULL;
    
    /* Check the Card capacity in term of Logical number of blocks */
    if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
    {
      BlockAdd *= 512;
    }
    
    /* Set Block Size for Card */ 
    errorstate = SDMMC_CmdBlockLength(hmmc->Instance, BLOCKSIZE);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)
    {
      hmmc->Context = (MMC_CONTEXT_WRITE_MULTIPLE_BLOCK | MMC_CONTEXT_DMA);
      
      /* Write Multi Block command */ 
      errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, BlockAdd);
    }
    else
    {
      hmmc->Context = (MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_DMA);
      
      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, BlockAdd);
    }
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Enable SDMMC DMA transfer */
    __HAL_MMC_DMA_ENABLE(hmmc);
    
    /* Enable the DMA Channel */
    HAL_DMA_Start_IT(hmmc->hdmatx, (uint32_t)pData, (uint32_t)&hmmc->Instance->FIFO, (uint32_t)(BLOCKSIZE * NumberOfBlocks)/4);
    
    /* Configure the MMC DPSM (Data Path State Machine) */ 
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = BLOCKSIZE * NumberOfBlocks;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_CARD;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_ENABLE;
    SDMMC_ConfigData(hmmc->Instance, &config);
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Erases the specified memory area of the given MMC card.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc: Pointer to MMC handle 
  * @param  BlockStartAdd: Start Block address
  * @param  BlockEndAdd: End Block address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Erase(MMC_HandleTypeDef *hmmc, uint32_t BlockStartAdd, uint32_t BlockEndAdd)
{
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    
    if(BlockEndAdd < BlockStartAdd)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
      return HAL_ERROR;
    }
    
    if(BlockEndAdd > 0xE88000)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_BUSY;
    
    /* Check if the card command class supports erase command */
    if(((hmmc->MmcCard.Class) & SDMMC_CCCC_ERASE) == 0U)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    if((SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);  
      hmmc->ErrorCode |= HAL_MMC_ERROR_LOCK_UNLOCK_FAILED;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    /* Send CMD35 MMC_ERASE_GRP_START with argument as addr  */
    errorstate = SDMMC_CmdEraseStartAdd(hmmc->Instance, BlockStartAdd);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Send CMD36 MMC_ERASE_GRP_END with argument as addr  */
    errorstate = SDMMC_CmdEraseEndAdd(hmmc->Instance, BlockEndAdd);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    /* Send CMD38 ERASE */
    errorstate = SDMMC_CmdErase(hmmc->Instance);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS); 
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
    }
    
    hmmc->State = HAL_MMC_STATE_READY;
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  This function handles MMC card interrupt request.
  * @param  hmmc: Pointer to MMC handle
  * @retval None
  */
void HAL_MMC_IRQHandler(MMC_HandleTypeDef *hmmc)
{
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  /* Check for SDMMC interrupt flags */
  if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_DATAEND) != RESET)
  {
    __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_FLAG_DATAEND); 
    
    __HAL_MMC_DISABLE_IT(hmmc, SDMMC_IT_DATAEND | SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT|\
                             SDMMC_IT_TXUNDERR| SDMMC_IT_RXOVERR);
    
    if((hmmc->Context & MMC_CONTEXT_IT) != RESET)
    {
      if(((hmmc->Context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) != RESET) || ((hmmc->Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK) != RESET))
      {
        errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
          hmmc->ErrorCode |= errorstate;
          HAL_MMC_ErrorCallback(hmmc);
        }
      }
      
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      
      hmmc->State = HAL_MMC_STATE_READY;
      if(((hmmc->Context & MMC_CONTEXT_READ_SINGLE_BLOCK) != RESET) || ((hmmc->Context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) != RESET))
      {
        HAL_MMC_RxCpltCallback(hmmc);
      }
      else
      {
        HAL_MMC_TxCpltCallback(hmmc);
      }
    }
    else if((hmmc->Context & MMC_CONTEXT_DMA) != RESET)
    {
      if((hmmc->Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK) != RESET)
      {
        errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
          hmmc->ErrorCode |= errorstate;
          HAL_MMC_ErrorCallback(hmmc);
        }
      }
      if(((hmmc->Context & MMC_CONTEXT_READ_SINGLE_BLOCK) == RESET) && ((hmmc->Context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) == RESET))
      {
        /* Disable the DMA transfer for transmit request by setting the DMAEN bit
        in the MMC DCTRL register */
        hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDMMC_DCTRL_DMAEN);
        
        hmmc->State = HAL_MMC_STATE_READY;
        
        HAL_MMC_TxCpltCallback(hmmc);
      }
    }
  }
  
  else if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_TXFIFOHE) != RESET)
  {
    __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_FLAG_TXFIFOHE);
    
    MMC_Write_IT(hmmc);
  }
  
  else if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_RXFIFOHF) != RESET)
  {
    __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_FLAG_RXFIFOHF);
    
    MMC_Read_IT(hmmc);
  }
  
  else if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_RXOVERR | SDMMC_IT_TXUNDERR) != RESET)
  {
    /* Set Error code */
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_DCRCFAIL) != RESET)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL; 
    }
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_DTIMEOUT) != RESET)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT; 
    }
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_RXOVERR) != RESET)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_RX_OVERRUN; 
    }
    if(__HAL_MMC_GET_FLAG(hmmc, SDMMC_IT_TXUNDERR) != RESET)
    {
      hmmc->ErrorCode |= HAL_MMC_ERROR_TX_UNDERRUN; 
    }
	
    /* Clear All flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
    
    /* Disable all interrupts */
    __HAL_MMC_DISABLE_IT(hmmc, SDMMC_IT_DATAEND | SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT|\
                             SDMMC_IT_TXUNDERR| SDMMC_IT_RXOVERR);
    
    if((hmmc->Context & MMC_CONTEXT_DMA) != RESET)
    {
      /* Abort the MMC DMA Streams */
      if(hmmc->hdmatx != NULL)
      {
        /* Set the DMA Tx abort callback */
        hmmc->hdmatx->XferAbortCallback = MMC_DMATxAbort;
        /* Abort DMA in IT mode */
        if(HAL_DMA_Abort_IT(hmmc->hdmatx) != HAL_OK)
        {
          MMC_DMATxAbort(hmmc->hdmatx);
        }
      }
      else if(hmmc->hdmarx != NULL)
      {
        /* Set the DMA Rx abort callback */
        hmmc->hdmarx->XferAbortCallback = MMC_DMARxAbort;
        /* Abort DMA in IT mode */
        if(HAL_DMA_Abort_IT(hmmc->hdmarx) != HAL_OK)
        {
          MMC_DMARxAbort(hmmc->hdmarx);
        }
      }
      else
      {
        hmmc->ErrorCode = HAL_MMC_ERROR_NONE;
        hmmc->State = HAL_MMC_STATE_READY;
        HAL_MMC_AbortCallback(hmmc);
      }
    }
    else if((hmmc->Context & MMC_CONTEXT_IT) != RESET)
    {
      /* Set the MMC state to ready to be able to start again the process */
      hmmc->State = HAL_MMC_STATE_READY;
      HAL_MMC_ErrorCallback(hmmc);
    }
  }   
}

/**
  * @brief return the MMC state
  * @param hmmc: Pointer to mmc handle
  * @retval HAL state
  */
HAL_MMC_StateTypeDef HAL_MMC_GetState(MMC_HandleTypeDef *hmmc)
{
  return hmmc->State;
}

/**
* @brief  Return the MMC error code
* @param  hmmc : Pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
* @retval MMC Error Code
*/
uint32_t HAL_MMC_GetError(MMC_HandleTypeDef *hmmc)
{
  return hmmc->ErrorCode;
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hmmc: Pointer to MMC handle
  * @retval None
  */
 __weak void HAL_MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_TxCpltCallback can be implemented in the user file
   */
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hmmc: Pointer MMC handle
  * @retval None
  */
__weak void HAL_MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);
 
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_ErrorCallback can be implemented in the user file
   */
}

/**
  * @brief MMC error callbacks
  * @param hmmc: Pointer MMC handle
  * @retval None
  */
__weak void HAL_MMC_ErrorCallback(MMC_HandleTypeDef *hmmc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);
 
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_ErrorCallback can be implemented in the user file
   */ 
}

/**
  * @brief MMC Abort callbacks
  * @param hmmc: Pointer MMC handle
  * @retval None
  */
__weak void HAL_MMC_AbortCallback(MMC_HandleTypeDef *hmmc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);
 
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_ErrorCallback can be implemented in the user file
   */ 
}


/**
  * @}
  */

/** @addtogroup MMC_Exported_Functions_Group3
 *  @brief   management functions 
 *
@verbatim   
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control the MMC card 
    operations and get the related information

@endverbatim
  * @{
  */

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CID register.
  * @param  hmmc: Pointer to MMC handle
  * @param  pCID: Pointer to a HAL_MMC_CIDTypedef structure that  
  *         contains all CID register parameters 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardCID(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCIDTypeDef *pCID)
{
  uint32_t tmp = 0;
  
  /* Byte 0 */
  tmp = (uint8_t)((hmmc->CID[0] & 0xFF000000U) >> 24);
  pCID->ManufacturerID = tmp;
  
  /* Byte 1 */
  tmp = (uint8_t)((hmmc->CID[0] & 0x00FF0000) >> 16);
  pCID->OEM_AppliID = tmp << 8;
  
  /* Byte 2 */
  tmp = (uint8_t)((hmmc->CID[0] & 0x000000FF00) >> 8);
  pCID->OEM_AppliID |= tmp;
  
  /* Byte 3 */
  tmp = (uint8_t)(hmmc->CID[0] & 0x000000FF);
  pCID->ProdName1 = tmp << 24;
  
  /* Byte 4 */
  tmp = (uint8_t)((hmmc->CID[1] & 0xFF000000U) >> 24);
  pCID->ProdName1 |= tmp << 16;
  
  /* Byte 5 */
  tmp = (uint8_t)((hmmc->CID[1] & 0x00FF0000) >> 16);
  pCID->ProdName1 |= tmp << 8;
  
  /* Byte 6 */
  tmp = (uint8_t)((hmmc->CID[1] & 0x0000FF00) >> 8);
  pCID->ProdName1 |= tmp;
  
  /* Byte 7 */
  tmp = (uint8_t)(hmmc->CID[1] & 0x000000FF);
  pCID->ProdName2 = tmp;
  
  /* Byte 8 */
  tmp = (uint8_t)((hmmc->CID[2] & 0xFF000000U) >> 24);
  pCID->ProdRev = tmp;
  
  /* Byte 9 */
  tmp = (uint8_t)((hmmc->CID[2] & 0x00FF0000) >> 16);
  pCID->ProdSN = tmp << 24;
  
  /* Byte 10 */
  tmp = (uint8_t)((hmmc->CID[2] & 0x0000FF00) >> 8);
  pCID->ProdSN |= tmp << 16;
  
  /* Byte 11 */
  tmp = (uint8_t)(hmmc->CID[2] & 0x000000FF);
  pCID->ProdSN |= tmp << 8;
  
  /* Byte 12 */
  tmp = (uint8_t)((hmmc->CID[3] & 0xFF000000U) >> 24);
  pCID->ProdSN |= tmp;
  
  /* Byte 13 */
  tmp = (uint8_t)((hmmc->CID[3] & 0x00FF0000) >> 16);
  pCID->Reserved1   |= (tmp & 0xF0) >> 4;
  pCID->ManufactDate = (tmp & 0x0F) << 8;
  
  /* Byte 14 */
  tmp = (uint8_t)((hmmc->CID[3] & 0x0000FF00) >> 8);
  pCID->ManufactDate |= tmp;
  
  /* Byte 15 */
  tmp = (uint8_t)(hmmc->CID[3] & 0x000000FF);
  pCID->CID_CRC   = (tmp & 0xFE) >> 1;
  pCID->Reserved2 = 1;

  return HAL_OK;
}

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CSD register.
  * @param  hmmc: Pointer to MMC handle
  * @param  pCSD: Pointer to a HAL_MMC_CardInfoTypeDef structure that  
  *         contains all CSD register parameters  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardCSD(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCSDTypeDef *pCSD)
{
  uint32_t tmp = 0;
  
  /* Byte 0 */
  tmp = (hmmc->CSD[0] & 0xFF000000U) >> 24;
  pCSD->CSDStruct      = (uint8_t)((tmp & 0xC0) >> 6);
  pCSD->SysSpecVersion = (uint8_t)((tmp & 0x3C) >> 2);
  pCSD->Reserved1      = tmp & 0x03;
  
  /* Byte 1 */
  tmp = (hmmc->CSD[0] & 0x00FF0000) >> 16;
  pCSD->TAAC = (uint8_t)tmp;
  
  /* Byte 2 */
  tmp = (hmmc->CSD[0] & 0x0000FF00) >> 8;
  pCSD->NSAC = (uint8_t)tmp;
  
  /* Byte 3 */
  tmp = hmmc->CSD[0] & 0x000000FF;
  pCSD->MaxBusClkFrec = (uint8_t)tmp;
  
  /* Byte 4 */
  tmp = (hmmc->CSD[1] & 0xFF000000U) >> 24;
  pCSD->CardComdClasses = (uint16_t)(tmp << 4);
  
  /* Byte 5 */
  tmp = (hmmc->CSD[1] & 0x00FF0000U) >> 16;
  pCSD->CardComdClasses |= (uint16_t)((tmp & 0xF0) >> 4);
  pCSD->RdBlockLen       = (uint8_t)(tmp & 0x0F);
  
  /* Byte 6 */
  tmp = (hmmc->CSD[1] & 0x0000FF00U) >> 8;
  pCSD->PartBlockRead   = (uint8_t)((tmp & 0x80) >> 7);
  pCSD->WrBlockMisalign = (uint8_t)((tmp & 0x40) >> 6);
  pCSD->RdBlockMisalign = (uint8_t)((tmp & 0x20) >> 5);
  pCSD->DSRImpl         = (uint8_t)((tmp & 0x10) >> 4);
  pCSD->Reserved2       = 0; /*!< Reserved */
  
  pCSD->DeviceSize = (tmp & 0x03) << 10;
  
  /* Byte 7 */
  tmp = (uint8_t)(hmmc->CSD[1] & 0x000000FFU);
  pCSD->DeviceSize |= (tmp) << 2;
  
  /* Byte 8 */
  tmp = (uint8_t)((hmmc->CSD[2] & 0xFF000000U) >> 24);
  pCSD->DeviceSize |= (tmp & 0xC0) >> 6;
  
  pCSD->MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
  pCSD->MaxRdCurrentVDDMax = (tmp & 0x07);
  
  /* Byte 9 */
  tmp = (uint8_t)((hmmc->CSD[2] & 0x00FF0000U) >> 16);
  pCSD->MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
  pCSD->MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
  pCSD->DeviceSizeMul      = (tmp & 0x03) << 1;
  /* Byte 10 */
  tmp = (uint8_t)((hmmc->CSD[2] & 0x0000FF00U) >> 8);
  pCSD->DeviceSizeMul |= (tmp & 0x80) >> 7;
  
  hmmc->MmcCard.BlockNbr  = (pCSD->DeviceSize + 1) ;
  hmmc->MmcCard.BlockNbr *= (1 << (pCSD->DeviceSizeMul + 2));
  hmmc->MmcCard.BlockSize = 1 << (pCSD->RdBlockLen);
  
  hmmc->MmcCard.LogBlockNbr =  (hmmc->MmcCard.BlockNbr) * ((hmmc->MmcCard.BlockSize) / 512); 
  hmmc->MmcCard.LogBlockSize = 512;
  
  pCSD->EraseGrSize = (tmp & 0x40) >> 6;
  pCSD->EraseGrMul  = (tmp & 0x3F) << 1;
  
  /* Byte 11 */
  tmp = (uint8_t)(hmmc->CSD[2] & 0x000000FF);
  pCSD->EraseGrMul     |= (tmp & 0x80) >> 7;
  pCSD->WrProtectGrSize = (tmp & 0x7F);
  
  /* Byte 12 */
  tmp = (uint8_t)((hmmc->CSD[3] & 0xFF000000U) >> 24);
  pCSD->WrProtectGrEnable = (tmp & 0x80) >> 7;
  pCSD->ManDeflECC        = (tmp & 0x60) >> 5;
  pCSD->WrSpeedFact       = (tmp & 0x1C) >> 2;
  pCSD->MaxWrBlockLen     = (tmp & 0x03) << 2;
  
  /* Byte 13 */
  tmp = (uint8_t)((hmmc->CSD[3] & 0x00FF0000) >> 16);
  pCSD->MaxWrBlockLen      |= (tmp & 0xC0) >> 6;
  pCSD->WriteBlockPaPartial = (tmp & 0x20) >> 5;
  pCSD->Reserved3           = 0;
  pCSD->ContentProtectAppli = (tmp & 0x01);
  
  /* Byte 14 */
  tmp = (uint8_t)((hmmc->CSD[3] & 0x0000FF00) >> 8);
  pCSD->FileFormatGrouop = (tmp & 0x80) >> 7;
  pCSD->CopyFlag         = (tmp & 0x40) >> 6;
  pCSD->PermWrProtect    = (tmp & 0x20) >> 5;
  pCSD->TempWrProtect    = (tmp & 0x10) >> 4;
  pCSD->FileFormat       = (tmp & 0x0C) >> 2;
  pCSD->ECC              = (tmp & 0x03);
  
  /* Byte 15 */
  tmp = (uint8_t)(hmmc->CSD[3] & 0x000000FF);
  pCSD->CSD_CRC   = (tmp & 0xFE) >> 1;
  pCSD->Reserved4 = 1;
  
  return HAL_OK;
}

/**
  * @brief  Gets the MMC card info.
  * @param  hmmc: Pointer to MMC handle      
  * @param  pCardInfo: Pointer to the HAL_MMC_CardInfoTypeDef structure that 
  *         will contain the MMC card status information 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardInfo(MMC_HandleTypeDef *hmmc, HAL_MMC_CardInfoTypeDef *pCardInfo)
{
  pCardInfo->CardType     = (uint32_t)(hmmc->MmcCard.CardType);
  pCardInfo->Class        = (uint32_t)(hmmc->MmcCard.Class);
  pCardInfo->RelCardAdd   = (uint32_t)(hmmc->MmcCard.RelCardAdd);
  pCardInfo->BlockNbr     = (uint32_t)(hmmc->MmcCard.BlockNbr);
  pCardInfo->BlockSize    = (uint32_t)(hmmc->MmcCard.BlockSize);
  pCardInfo->LogBlockNbr  = (uint32_t)(hmmc->MmcCard.LogBlockNbr);
  pCardInfo->LogBlockSize = (uint32_t)(hmmc->MmcCard.LogBlockSize);
  
  return HAL_OK;
}

/**
  * @brief  Enables wide bus operation for the requested card if supported by 
  *         card.
  * @param  hmmc: Pointer to MMC handle       
  * @param  WideMode: Specifies the MMC card wide bus mode 
  *          This parameter can be one of the following values:
  *            @arg SDMMC_BUS_WIDE_8B: 8-bit data transfer
  *            @arg SDMMC_BUS_WIDE_4B: 4-bit data transfer
  *            @arg SDMMC_BUS_WIDE_1B: 1-bit data transfer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ConfigWideBusOperation(MMC_HandleTypeDef *hmmc, uint32_t WideMode)
{
  __IO uint32_t count = 0;
  SDMMC_InitTypeDef Init;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  uint32_t response = 0, busy = 0;
	
	/* Check the parameters */
  assert_param(IS_SDMMC_BUS_WIDE(WideMode));
	
  /* Chnage Satte */
  hmmc->State = HAL_MMC_STATE_BUSY;
  
  /* Update Clock for Bus mode update */
  Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
  Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  Init.BusWide             = WideMode;
  Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  Init.ClockDiv            = SDMMC_INIT_CLK_DIV;
  /* Initialize SDMMC*/
  SDMMC_Init(hmmc->Instance, Init); 
    
  if(WideMode == SDMMC_BUS_WIDE_8B)
  {
    errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70200);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      hmmc->ErrorCode |= errorstate;
    }
  }
  else if(WideMode == SDMMC_BUS_WIDE_4B)
  {
    errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70100);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      hmmc->ErrorCode |= errorstate;
    }
  }
  else if(WideMode == SDMMC_BUS_WIDE_1B)
  {
    errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70000);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      hmmc->ErrorCode |= errorstate;
    }
  }
  else
  {
    /* WideMode is not a valid argument*/
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
  }
  
  /* Check for switch error and violation of the trial number of sending CMD 13 */
  while(busy == 0)
  {
    if(count++ == SDMMC_MAX_TRIAL)
    {
      hmmc->State = HAL_MMC_STATE_READY;
      hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
      return HAL_ERROR;
    }
    
    /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
    errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16));
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      hmmc->ErrorCode |= errorstate;
    }
    
    /* Get command response */
    response = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP1);
    
    /* Get operating voltage*/
    busy = (((response >> 7) == 1) ? 0 : 1);
  }
      
  /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
  count = SDMMC_DATATIMEOUT;
  while((response & 0x00000100) == 0)
  {
    if(count-- == 0)
    {
      hmmc->State = HAL_MMC_STATE_READY;
      hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
      return HAL_ERROR;
    }
    
    /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
    errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16));
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      hmmc->ErrorCode |= errorstate;
    }
    
    /* Get command response */
    response = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP1);
  }
  
  if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
  {
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
    hmmc->State = HAL_MMC_STATE_READY;
    return HAL_ERROR;
  }
  else
  {
    /* Configure the SDMMC peripheral */
    Init.ClockEdge           = hmmc->Init.ClockEdge;
    Init.ClockBypass         = hmmc->Init.ClockBypass;
    Init.ClockPowerSave      = hmmc->Init.ClockPowerSave;
    Init.BusWide             = WideMode;
    Init.HardwareFlowControl = hmmc->Init.HardwareFlowControl;
    Init.ClockDiv            = hmmc->Init.ClockDiv;
    SDMMC_Init(hmmc->Instance, Init);
  }

  /* Change State */
  hmmc->State = HAL_MMC_STATE_READY;
  
  return HAL_OK;
}


/**
  * @brief  Gets the current mmc card data state.
  * @param  hmmc: pointer to MMC handle
  * @retval Card state
  */
HAL_MMC_CardStateTypeDef HAL_MMC_GetCardState(MMC_HandleTypeDef *hmmc)
{
  HAL_MMC_CardStateTypeDef cardstate =  HAL_MMC_CARD_TRANSFER;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  uint32_t resp1 = 0;
  
  errorstate = MMC_SendStatus(hmmc, &resp1);
  if(errorstate != HAL_OK)
  {
    hmmc->ErrorCode |= errorstate;
  }

  cardstate = (HAL_MMC_CardStateTypeDef)((resp1 >> 9) & 0x0F);
  
  return cardstate;
}

/**
  * @brief  Abort the current transfer and disable the MMC.
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *                the configuration information for MMC module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Abort(MMC_HandleTypeDef *hmmc)
{
  HAL_MMC_CardStateTypeDef CardState;
  
  /* DIsable All interrupts */
  __HAL_MMC_DISABLE_IT(hmmc, SDMMC_IT_DATAEND | SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT|\
                           SDMMC_IT_TXUNDERR| SDMMC_IT_RXOVERR);
  
  /* Clear All flags */
  __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
  
  if((hmmc->hdmatx != NULL) || (hmmc->hdmarx != NULL))
  {
    /* Disable the MMC DMA request */
    hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDMMC_DCTRL_DMAEN);
    
    /* Abort the MMC DMA Tx Stream */
    if(hmmc->hdmatx != NULL)
    {
      HAL_DMA_Abort(hmmc->hdmatx);
    }
    /* Abort the MMC DMA Rx Stream */
    if(hmmc->hdmarx != NULL)
    {
      HAL_DMA_Abort(hmmc->hdmarx);
    }
  }
  
  hmmc->State = HAL_MMC_STATE_READY;
  CardState = HAL_MMC_GetCardState(hmmc);
  if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))
  {
    hmmc->ErrorCode = SDMMC_CmdStopTransfer(hmmc->Instance);
  }
  if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

/**
  * @brief  Abort the current transfer and disable the MMC (IT mode).
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *                the configuration information for MMC module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Abort_IT(MMC_HandleTypeDef *hmmc)
{
  HAL_MMC_CardStateTypeDef CardState;
    
  /* DIsable All interrupts */
  __HAL_MMC_DISABLE_IT(hmmc, SDMMC_IT_DATAEND | SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT|\
                           SDMMC_IT_TXUNDERR| SDMMC_IT_RXOVERR);
  
  /* Clear All flags */
  __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
  
  if((hmmc->hdmatx != NULL) || (hmmc->hdmarx != NULL))
  {
    /* Disable the MMC DMA request */
    hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDMMC_DCTRL_DMAEN);
    
    /* Abort the MMC DMA Tx Stream */
    if(hmmc->hdmatx != NULL)
    {
      hmmc->hdmatx->XferAbortCallback =  MMC_DMATxAbort;
      if(HAL_DMA_Abort_IT(hmmc->hdmatx) != HAL_OK)
      {
        hmmc->hdmatx = NULL;
      }
    }
    /* Abort the MMC DMA Rx Stream */
    if(hmmc->hdmarx != NULL)
    {
      hmmc->hdmarx->XferAbortCallback =  MMC_DMARxAbort;
      if(HAL_DMA_Abort_IT(hmmc->hdmarx) != HAL_OK)
      {
        hmmc->hdmarx = NULL;
      }
    }
  }
  
  /* No transfer ongoing on both DMA channels*/
  if((hmmc->hdmatx == NULL) && (hmmc->hdmarx == NULL))
  {
    CardState = HAL_MMC_GetCardState(hmmc);
    hmmc->State = HAL_MMC_STATE_READY;
    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))
    {
      hmmc->ErrorCode = SDMMC_CmdStopTransfer(hmmc->Instance);
    }
    if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
    {
      return HAL_ERROR;
    }
    else
    {
      HAL_MMC_AbortCallback(hmmc);
    }
  }
  
  return HAL_OK;
}

/**
  * @}
  */
  
/**
  * @}
  */
  
/* Private function ----------------------------------------------------------*/  
/** @addtogroup MMC_Private_Functions
  * @{
  */

/**
  * @brief  DMA MMC transmit process complete callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMATransmitCplt(DMA_HandleTypeDef *hdma)     
{
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);
  
  /* Enable DATAEND Interrupt */
  __HAL_MMC_ENABLE_IT(hmmc, (SDMMC_IT_DATAEND));
}

/**
  * @brief  DMA MMC receive process complete callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  
{
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  /* Send stop command in multiblock write */
  if(hmmc->Context == (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_DMA))
  {
    errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      hmmc->ErrorCode |= errorstate;
      HAL_MMC_ErrorCallback(hmmc);
    }
  }
  
  /* Disable the DMA transfer for transmit request by setting the DMAEN bit
  in the MMC DCTRL register */
  hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDMMC_DCTRL_DMAEN);
  
  /* Clear all the static flags */
  __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
  
  hmmc->State = HAL_MMC_STATE_READY;

  HAL_MMC_RxCpltCallback(hmmc);
}

/**
  * @brief  DMA MMC communication error callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMAError(DMA_HandleTypeDef *hdma)   
{
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);
  HAL_MMC_CardStateTypeDef CardState;
  
  /* if DMA error is FIFO error ignore it */
  if(HAL_DMA_GetError(hdma) != HAL_DMA_ERROR_FE)
  {
    if((hmmc->hdmarx->ErrorCode == HAL_DMA_ERROR_TE) || (hmmc->hdmatx->ErrorCode == HAL_DMA_ERROR_TE))
    {
      /* Clear All flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      
      /* Disable All interrupts */
      __HAL_MMC_DISABLE_IT(hmmc, SDMMC_IT_DATAEND | SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT|\
        SDMMC_IT_TXUNDERR| SDMMC_IT_RXOVERR);
      
      hmmc->ErrorCode |= HAL_MMC_ERROR_DMA;
      CardState = HAL_MMC_GetCardState(hmmc);
      if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))
      {
        hmmc->ErrorCode |= SDMMC_CmdStopTransfer(hmmc->Instance);
      }
      
      hmmc->State= HAL_MMC_STATE_READY;
    }
    
    HAL_MMC_ErrorCallback(hmmc);
  }
}

/**
  * @brief  DMA MMC Tx Abort callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMATxAbort(DMA_HandleTypeDef *hdma)   
{
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);
  HAL_MMC_CardStateTypeDef CardState;
  
  if(hmmc->hdmatx != NULL)
  {
    hmmc->hdmatx = NULL;
  }
  
  /* All DMA channels are aborted */
  if(hmmc->hdmarx == NULL)
  {
    CardState = HAL_MMC_GetCardState(hmmc);
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;
    hmmc->State = HAL_MMC_STATE_READY;
    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))
    {
      hmmc->ErrorCode |= SDMMC_CmdStopTransfer(hmmc->Instance);
      
      if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
      {
        HAL_MMC_AbortCallback(hmmc);
      }
      else
      {
        HAL_MMC_ErrorCallback(hmmc);
      }
    }
  }
}

/**
  * @brief  DMA MMC Rx Abort callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMARxAbort(DMA_HandleTypeDef *hdma)   
{
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);
  HAL_MMC_CardStateTypeDef CardState;
  
  if(hmmc->hdmarx != NULL)
  {
    hmmc->hdmarx = NULL;
  }
  
  /* All DMA channels are aborted */
  if(hmmc->hdmatx == NULL)
  {
    CardState = HAL_MMC_GetCardState(hmmc);
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;
    hmmc->State = HAL_MMC_STATE_READY;
    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))
    {
      hmmc->ErrorCode |= SDMMC_CmdStopTransfer(hmmc->Instance);
      
      if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
      {
        HAL_MMC_AbortCallback(hmmc);
      }
      else
      {
        HAL_MMC_ErrorCallback(hmmc);
      }
    }
  }
}

/**
  * @brief  Initializes the mmc card.
  * @param  hmmc: Pointer to MMC handle
  * @retval MMC Card error state
  */
static uint32_t MMC_InitCard(MMC_HandleTypeDef *hmmc)
{
  HAL_MMC_CardCSDTypeDef CSD;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  uint16_t mmc_rca = 1;
  
  /* Check the power State */
  if(SDMMC_GetPowerState(hmmc->Instance) == 0) 
  {
    /* Power off */
    return HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
  }
  
  /* Send CMD2 ALL_SEND_CID */
  errorstate = SDMMC_CmdSendCID(hmmc->Instance);
  if(errorstate != HAL_MMC_ERROR_NONE)
  {
    return errorstate;
  }
  else
  {
    /* Get Card identification number data */
    hmmc->CID[0] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP1);
    hmmc->CID[1] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP2);
    hmmc->CID[2] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP3);
    hmmc->CID[3] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP4);
  }
  
  /* Send CMD3 SET_REL_ADDR with argument 0 */
  /* MMC Card publishes its RCA. */
  errorstate = SDMMC_CmdSetRelAdd(hmmc->Instance, &mmc_rca);
  if(errorstate != HAL_MMC_ERROR_NONE)
  {
    return errorstate;
  }
  
  /* Get the MMC card RCA */
  hmmc->MmcCard.RelCardAdd = mmc_rca;
  
  /* Send CMD9 SEND_CSD with argument as card's RCA */
  errorstate = SDMMC_CmdSendCSD(hmmc->Instance, (uint32_t)(hmmc->MmcCard.RelCardAdd << 16U));
  if(errorstate != HAL_MMC_ERROR_NONE)
  {
    return errorstate;
  }
  else
  {
    /* Get Card Specific Data */
    hmmc->CSD[0U] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP1);
    hmmc->CSD[1U] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP2);
    hmmc->CSD[2U] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP3);
    hmmc->CSD[3U] = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP4);
  }
  
  /* Get the Card Class */
  hmmc->MmcCard.Class = (SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP2) >> 20);
  
  /* Get CSD parameters */
  HAL_MMC_GetCardCSD(hmmc, &CSD);

  /* Select the Card */
  errorstate = SDMMC_CmdSelDesel(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16));
  if(errorstate != HAL_MMC_ERROR_NONE)
  {
    return errorstate;
  }
  /* Configure SDMMC peripheral interface */     
  SDMMC_Init(hmmc->Instance, hmmc->Init);

  /* All cards are initialized */
  return HAL_MMC_ERROR_NONE;
}

/**
  * @brief  Enquires cards about their operating voltage and configures clock
  *         controls and stores MMC information that will be needed in future
  *         in the MMC handle.
  * @param  hmmc: Pointer to MMC handle
  * @retval error state
  */
static uint32_t MMC_PowerON(MMC_HandleTypeDef *hmmc)
{
  __IO uint32_t count = 0;
  uint32_t response = 0, validvoltage = 0;
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  /* CMD0: GO_IDLE_STATE */
  errorstate = SDMMC_CmdGoIdleState(hmmc->Instance);
  if(errorstate != HAL_MMC_ERROR_NONE)
  {
    return errorstate;
  }
  
  while(validvoltage == 0)
  {
    if(count++ == SDMMC_MAX_VOLT_TRIAL)
    {
      return HAL_MMC_ERROR_INVALID_VOLTRANGE;
    }
    
    /* SEND CMD1 APP_CMD with MMC_HIGH_VOLTAGE_RANGE(0xC0FF8000) as argument */
    errorstate = SDMMC_CmdOpCondition(hmmc->Instance, eMMC_HIGH_VOLTAGE_RANGE);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
      return HAL_MMC_ERROR_UNSUPPORTED_FEATURE;
    }
    
    /* Get command response */
    response = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP1);
    
    /* Get operating voltage*/
    validvoltage = (((response >> 31) == 1) ? 1 : 0);
  }
  
  /* When power routine is finished and command returns valid voltage */
  if ((response & MMC_HIGH_VOLTAGE_RANGE) == MMC_HIGH_VOLTAGE_RANGE)
  {
    /* When voltage range of the card is within 2.7V and 3.6V */
    hmmc->MmcCard.CardType = MMC_HIGH_VOLTAGE_CARD;
  }
  else
  {
    /* When voltage range of the card is within 1.65V and 1.95V or 2.7V and 3.6V */
    hmmc->MmcCard.CardType = MMC_DUAL_VOLTAGE_CARD;
  }
  
  return HAL_MMC_ERROR_NONE;
}

/**
  * @brief  Turns the SDMMC output signals off.
  * @param  hmmc: Pointer to MMC handle
  * @retval HAL status
  */
static HAL_StatusTypeDef MMC_PowerOFF(MMC_HandleTypeDef *hmmc)
{
  /* Set Power State to OFF */
  SDMMC_PowerState_OFF(hmmc->Instance);
  
  return HAL_OK;
}

/**
  * @brief  Returns the current card's status.
  * @param  hmmc: Pointer to MMC handle
  * @param  pCardStatus: pointer to the buffer that will contain the MMC card 
  *         status (Card Status register)  
  * @retval error state
  */
static uint32_t MMC_SendStatus(MMC_HandleTypeDef *hmmc, uint32_t *pCardStatus)
{
  uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
  if(pCardStatus == NULL)
  {
    return HAL_MMC_ERROR_PARAM;
  }
  
  /* Send Status command */
  errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(hmmc->MmcCard.RelCardAdd << 16));
  if(errorstate != HAL_OK)
  {
    return errorstate;
  }
  
  /* Get MMC card status */
  *pCardStatus = SDMMC_GetResponse(hmmc->Instance, SDMMC_RESP1);
  
  return HAL_MMC_ERROR_NONE;
}

/**
  * @brief  Wrap up reading in non-blocking mode.
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval HAL status
  */
static HAL_StatusTypeDef MMC_Read_IT(MMC_HandleTypeDef *hmmc)
{
  uint32_t count = 0;
  uint32_t* tmp;

  tmp = (uint32_t*)hmmc->pRxBuffPtr;
  
  /* Read data from SDMMC Rx FIFO */
  for(count = 0; count < 8; count++)
  {
    *(tmp + count) = SDMMC_ReadFIFO(hmmc->Instance);
  }
  
  hmmc->pRxBuffPtr += 8;
  
  return HAL_OK;
}

/**
  * @brief  Wrap up writing in non-blocking mode.
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval HAL status
  */
static HAL_StatusTypeDef MMC_Write_IT(MMC_HandleTypeDef *hmmc)
{
  uint32_t count = 0;
  uint32_t* tmp;
  
  tmp = (uint32_t*)hmmc->pTxBuffPtr;
  
  /* Write data to SDMMC Tx FIFO */
  for(count = 0; count < 8; count++)
  {
    SDMMC_WriteFIFO(hmmc->Instance, (tmp + count));
  }
  
  hmmc->pTxBuffPtr += 8;
  
  return HAL_OK;
}

/**
  * @}
  */

#endif /* HAL_SD_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
