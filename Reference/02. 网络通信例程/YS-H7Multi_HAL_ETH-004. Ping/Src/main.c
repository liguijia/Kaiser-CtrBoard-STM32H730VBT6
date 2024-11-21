/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2018-09-01
  * 功    能: LWIP - DHCP
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-H7Multi使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
  
/* 包含头文件 ----------------------------------------------------------------*/

#include "main.h"
#include "stm32h7xx_hal.h"
#include "string.h"

#include "bsp_debug_usart.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"
#include "lwip/dhcp.h"

/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
uint8_t aRxBuffer;

/* 扩展变量 ------------------------------------------------------------------*/
extern __IO uint8_t DHCP_state;


/* 私有函数原形 --------------------------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);

/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{

  CPU_CACHE_Enable();

  /* 复位所有外设，初始化Flash接口和系统滴答定时器(HSI) */
  HAL_Init();
  
  /* 配置系统时钟 */
  SystemClock_Config();

  /* USART串口初始化 */
  MX_DEBUG_UART_Init();
  
  /* 调用格式化输出函数打印输出数据 */
  printf("YS-H7Multi网络测试，基本PING测试\n");

  /* 配置网络接口 */
  Netif_Config();
    
  /* 无限循环 */
  while (1)
  {
    /* 超时处理 */
    sys_check_timeouts();
    
#if LWIP_NETIF_LINK_CALLBACK
    Ethernet_Link_Periodic_Handle(&gnetif);
#endif
		
#ifdef USE_DHCP
    /* 定时调用DHCP获取IP地址 */
    DHCP_Periodic_Handle(&gnetif);
#endif    
  }
}

/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct={0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct={0};

  /* 使能PWR配置更新 */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

  /* 设置调压器输出电压级别1 */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* 等待D3区域VOS 输出准备就绪 */
  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) // 等待内核电源就绪
  {
    
  }
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  
  /* RCC 内部/外部晶振配置 ,用于CPU，AHB，APH总线时钟 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;// 外部晶振
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;//选择外部时钟晶振 HSE 25MHz
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;//使能PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;//PLL时钟源选择HSE
  RCC_OscInitStruct.PLL.PLLM = 5;  // 外部时钟2分频,得到5MHz 作为PLL时钟源
  RCC_OscInitStruct.PLL.PLLN = 160; // PLL 160倍频,得到800MHz PLLCLK
  RCC_OscInitStruct.PLL.PLLP = 2;  // PLLCLK 2分频 得到400MHz SYSCLK
  RCC_OscInitStruct.PLL.PLLQ = 2;  // PLLCLK 2分频 得到400MHz用于部分外设的时钟
  RCC_OscInitStruct.PLL.PLLR = 2;  // PLLCLK 2分频 得到400MHz用于部分外设的时钟
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;// PLL时钟输入范围4-8MHz
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;// PLL时钟输出范围192-836MHz
  RCC_OscInitStruct.PLL.PLLFRACN = 0;  // 此参数用于微调 PLL1 VCO 范围0~2^13-1
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* RCC CPU，AHB，APH总线时钟配置*/
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;//SYSCLK 时钟源400MHz
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;// SYSCLK 1分频 HCLK=400MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;  // HCLK 2分频，AHB = 200Mhz
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2; // APB3 2分频，APB3 = 100MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; // APB1 2分频，APB1 = 100MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; // APB2 2分频，APB2 = 100MHz
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; // APB4 2分频，APB4 = 100MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  
  
  /* 使能时钟安全机制 */
  HAL_RCC_EnableCSS();

  /* 滴答定时器配置1ms */
  // SystemCoreClock/1000    1ms中断一次
	// SystemCoreClock/100000	 10us中断一次
	// SystemCoreClock/1000000 1us中断一次
  HAL_SYSTICK_Config(SystemCoreClock/(1000));

  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* 系统滴答定时器中断优先级配置 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * 函数功能: 使能CPU L1-Cache
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void CPU_CACHE_Enable(void)
{
  /* 使能 I-Cache */
  SCB_EnableICache();

  /* 使能 D-Cache */
  SCB_EnableDCache();
 
}


/********** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *******END OF FILE******/

