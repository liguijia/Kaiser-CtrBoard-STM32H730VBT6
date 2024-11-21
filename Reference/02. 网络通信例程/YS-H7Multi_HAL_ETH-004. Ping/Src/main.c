/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2018-09-01
  * ��    ��: LWIP - DHCP
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-H7Multiʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
  
/* ����ͷ�ļ� ----------------------------------------------------------------*/

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

/* ˽�����Ͷ��� --------------------------------------------------------------*/

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/
uint8_t aRxBuffer;

/* ��չ���� ------------------------------------------------------------------*/
extern __IO uint8_t DHCP_state;


/* ˽�к���ԭ�� --------------------------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);

/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{

  CPU_CACHE_Enable();

  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ��(HSI) */
  HAL_Init();
  
  /* ����ϵͳʱ�� */
  SystemClock_Config();

  /* USART���ڳ�ʼ�� */
  MX_DEBUG_UART_Init();
  
  /* ���ø�ʽ�����������ӡ������� */
  printf("YS-H7Multi������ԣ�����PING����\n");

  /* ��������ӿ� */
  Netif_Config();
    
  /* ����ѭ�� */
  while (1)
  {
    /* ��ʱ���� */
    sys_check_timeouts();
    
#if LWIP_NETIF_LINK_CALLBACK
    Ethernet_Link_Periodic_Handle(&gnetif);
#endif
		
#ifdef USE_DHCP
    /* ��ʱ����DHCP��ȡIP��ַ */
    DHCP_Periodic_Handle(&gnetif);
#endif    
  }
}

/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct={0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct={0};

  /* ʹ��PWR���ø��� */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

  /* ���õ�ѹ�������ѹ����1 */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* �ȴ�D3����VOS ���׼������ */
  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) // �ȴ��ں˵�Դ����
  {
    
  }
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  
  /* RCC �ڲ�/�ⲿ�������� ,����CPU��AHB��APH����ʱ�� */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;// �ⲿ����
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;//ѡ���ⲿʱ�Ӿ��� HSE 25MHz
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;//ʹ��PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;//PLLʱ��Դѡ��HSE
  RCC_OscInitStruct.PLL.PLLM = 5;  // �ⲿʱ��2��Ƶ,�õ�5MHz ��ΪPLLʱ��Դ
  RCC_OscInitStruct.PLL.PLLN = 160; // PLL 160��Ƶ,�õ�800MHz PLLCLK
  RCC_OscInitStruct.PLL.PLLP = 2;  // PLLCLK 2��Ƶ �õ�400MHz SYSCLK
  RCC_OscInitStruct.PLL.PLLQ = 2;  // PLLCLK 2��Ƶ �õ�400MHz���ڲ��������ʱ��
  RCC_OscInitStruct.PLL.PLLR = 2;  // PLLCLK 2��Ƶ �õ�400MHz���ڲ��������ʱ��
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;// PLLʱ�����뷶Χ4-8MHz
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;// PLLʱ�������Χ192-836MHz
  RCC_OscInitStruct.PLL.PLLFRACN = 0;  // �˲�������΢�� PLL1 VCO ��Χ0~2^13-1
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* RCC CPU��AHB��APH����ʱ������*/
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;//SYSCLK ʱ��Դ400MHz
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;// SYSCLK 1��Ƶ HCLK=400MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;  // HCLK 2��Ƶ��AHB = 200Mhz
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2; // APB3 2��Ƶ��APB3 = 100MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; // APB1 2��Ƶ��APB1 = 100MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; // APB2 2��Ƶ��APB2 = 100MHz
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; // APB4 2��Ƶ��APB4 = 100MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  
  
  /* ʹ��ʱ�Ӱ�ȫ���� */
  HAL_RCC_EnableCSS();

  /* �δ�ʱ������1ms */
  // SystemCoreClock/1000    1ms�ж�һ��
	// SystemCoreClock/100000	 10us�ж�һ��
	// SystemCoreClock/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(SystemCoreClock/(1000));

  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ʹ��CPU L1-Cache
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void CPU_CACHE_Enable(void)
{
  /* ʹ�� I-Cache */
  SCB_EnableICache();

  /* ʹ�� D-Cache */
  SCB_EnableDCache();
 
}


/********** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *******END OF FILE******/

