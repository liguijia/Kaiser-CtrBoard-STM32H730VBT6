/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "octospi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BUZZER.h"
#include "w25q64.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define W25Qxx_NumByteToTest 32 * 1024 // 测试数据的长度，32K

int32_t OSPI_Status; // 检测标志位

uint8_t W25Qxx_WriteBuffer[W25Qxx_NumByteToTest]; //	写数据数组
uint8_t W25Qxx_ReadBuffer[W25Qxx_NumByteToTest];  //	读数据数组
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/***************************************************************************************************
 *	函 数 名: OSPI_W25Qxx_Test
 *	入口参数: 无
 *	返 回 值: OSPI_W25Qxx_OK - 测试成功并通过
 *	函数功能: 进行简单的读写测试，并计算速度
 *	说    明: 无
 ***************************************************************************************************/

int8_t OSPI_W25Qxx_Test(void) // Flash读写测试
{
    uint32_t i               = 0X8000; // 计数变量
    uint32_t W25Qxx_TestAddr = 0;      // 测试地址
    uint32_t ExecutionTime_Begin;      // 开始时间
    uint32_t ExecutionTime_End;        // 结束时间
    uint32_t ExecutionTime;            // 执行时间
    float ExecutionSpeed;              // 执行速度

    // 擦除 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    ExecutionTime_Begin = HAL_GetTick();                               // 获取 systick 当前时间，单位ms
    OSPI_Status         = OSPI_W25Qxx_BlockErase_32K(W25Qxx_TestAddr); // 擦除32K字节
    ExecutionTime_End   = HAL_GetTick();                               // 获取 systick 当前时间，单位ms

    ExecutionTime = ExecutionTime_End - ExecutionTime_Begin; // 计算擦除时间，单位ms

    if (OSPI_Status == OSPI_W25Qxx_OK) {
        printf("\r\nW25Q64 erase succeed, time: %d ms\r\n", ExecutionTime);
    } else {
        printf("\r\n erase error!!!!!  ERROR CODE:%d\r\n", OSPI_Status);
        while (1);
    }

    // 写入 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    for (i = 0; i < W25Qxx_NumByteToTest; i++) // 先将数据写入数组
    {
        W25Qxx_WriteBuffer[i] = i;
    }
    ExecutionTime_Begin = HAL_GetTick();                                                                      // 获取 systick 当前时间，单位ms
    OSPI_Status         = OSPI_W25Qxx_WriteBuffer(W25Qxx_WriteBuffer, W25Qxx_TestAddr, W25Qxx_NumByteToTest); // 写入数据
    ExecutionTime_End   = HAL_GetTick();                                                                      // 获取 systick 当前时间，单位ms

    ExecutionTime  = ExecutionTime_End - ExecutionTime_Begin;     // 计算擦除时间，单位ms
    ExecutionSpeed = (float)W25Qxx_NumByteToTest / ExecutionTime; // 计算写入速度，单位 KB/S
    if (OSPI_Status == OSPI_W25Qxx_OK) {
        printf("\r\nwrite succeed, data size: %d KB, time: %d ms, speed: %.2f KB/s\r\n", W25Qxx_NumByteToTest / 1024, ExecutionTime, ExecutionSpeed);
    } else {
        printf("\r\nwrite error!!!!!  error code: %d\r\n", OSPI_Status);
        while (1);
    }

    // 读取	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    ExecutionTime_Begin = HAL_GetTick();                                                                    // 获取 systick 当前时间，单位ms
    OSPI_Status         = OSPI_W25Qxx_ReadBuffer(W25Qxx_ReadBuffer, W25Qxx_TestAddr, W25Qxx_NumByteToTest); // 读取数据
    ExecutionTime_End   = HAL_GetTick();                                                                    // 获取 systick 当前时间，单位ms

    ExecutionTime  = ExecutionTime_End - ExecutionTime_Begin;                          // 计算擦除时间，单位ms
    ExecutionSpeed = (float)W25Qxx_NumByteToTest / 1024 / 1024 / ExecutionTime * 1000; // 计算读取速度，单位 MB/S

    if (OSPI_Status == OSPI_W25Qxx_OK) {
        printf("\r\nread succeed, size: %d KB, time: %d ms, speed: %.2f MB/s \r\n", W25Qxx_NumByteToTest / 1024, ExecutionTime, ExecutionSpeed);
    } else {
        printf("\r\nread error!!!!!  error code:%d\r\n", OSPI_Status);
        while (1);
    }
    // 数据校验 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    for (i = 0; i < W25Qxx_NumByteToTest; i++) // 验证读出的数据是否等于写入的数据
    {
        if (W25Qxx_WriteBuffer[i] != W25Qxx_ReadBuffer[i]) // 如果数据不相等，则返回0
        {
            printf("\r\ndata check error!!!!!pos: %d\r\n", i);
            while (1);
        }
    }
    printf("\r\ncheck pass!!!!!\r\n");

    // 读取整片Flash的数据，用以测试速度 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    printf("\r\n*****************************************************************************************************\r\n");
    printf("\r\nIn the above test, the data read is relatively small and takes a short time. In addition, the minimum unit of measurement is ms, and the calculated reading speed has a large error.\r\n");
    printf("\r\nNext, read the entire flash data to test the speed. The speed error obtained in this way is relatively small.\r\n");
    printf("\r\nread start>>>>\r\n");
    ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms

    for (i = 0; i < W25Qxx_FlashSize / (W25Qxx_NumByteToTest); i++) // 每次读取 W25Qxx_NumByteToTest 字节的数据
    {
        OSPI_Status     = OSPI_W25Qxx_ReadBuffer(W25Qxx_ReadBuffer, W25Qxx_TestAddr, W25Qxx_NumByteToTest); // 读取数据
        W25Qxx_TestAddr = W25Qxx_TestAddr + W25Qxx_NumByteToTest;
    }
    ExecutionTime_End = HAL_GetTick(); // 获取 systick 当前时间，单位ms

    ExecutionTime  = ExecutionTime_End - ExecutionTime_Begin;                      // 计算擦除时间，单位ms
    ExecutionSpeed = (float)W25Qxx_FlashSize / 1024 / 1024 / ExecutionTime * 1000; // 计算读取速度，单位 MB/S

    if (OSPI_Status == OSPI_W25Qxx_OK) {
        printf("\r\nread succeed, size: %d MB, time: %d ms, speed: %.2f MB/s \r\n", W25Qxx_FlashSize / 1024 / 1024, ExecutionTime, ExecutionSpeed);
    } else {
        printf("\r\nread error!!!!!  error code:%d\r\n", OSPI_Status);
        while (1);
    }

    return OSPI_W25Qxx_OK; // 测试通过
}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MPU Configuration--------------------------------------------------------*/
    MPU_Config();

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM4_Init();
    MX_OCTOSPI1_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    OSPI_W25Qxx_Init(); // 初始化OSPI和W25Q64

    OSPI_W25Qxx_Test(); // Flash读写测试
    BUZZER_0N(400);
    HAL_Delay(100);
    BUZZER_Stop();
    HAL_Delay(100);
    BUZZER_0N(500);
    HAL_Delay(100);
    BUZZER_Stop();
    BUZZER_0N(700);
    HAL_Delay(114);
    BUZZER_Stop();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)) {
            BUZZER_Stop();
        } else {
            BUZZER_0N(114);
        }

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = 64;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = 4;
    RCC_OscInitStruct.PLL.PLLN            = 34;
    RCC_OscInitStruct.PLL.PLLP            = 1;
    RCC_OscInitStruct.PLL.PLLQ            = 2;
    RCC_OscInitStruct.PLL.PLLR            = 2;
    RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL       = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN        = 3072;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* Disables the MPU */
    HAL_MPU_Disable();

    /** Initializes and configures the Region and the memory to be protected
     */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress      = 0x0;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /* Enables the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
