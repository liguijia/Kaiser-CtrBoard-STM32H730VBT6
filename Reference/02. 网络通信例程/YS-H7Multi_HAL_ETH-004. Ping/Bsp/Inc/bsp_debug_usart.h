/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2018-09-01
  * ��    ��: UART-API
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-H7Multiʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
#ifndef __BSP_DEBUG_USART_H__
#define __BSP_DEBUG_USART_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* �궨�� --------------------------------------------------------------------*/

/* ����ͨ�ŵ�ѡ�� */
#define	USART1_EN                     	         0
#define	USART2_EN                     	         0
#define	USART3_EN                     	         1
#define	USART6_EN                     	         0
#define	UART4_EN                      	         0
/******************** ѡ��USART1����ͨ�� ***********************/
#if USART1_EN == 1

#define DEBUG_USARTx                              USART1
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART1_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_14
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_15
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB

#define DEBUG_USARTx_AFx                          GPIO_AF4_USART1

#define DEBUG_USART_IRQn                          USART1_IRQn
#define DEBUG_USART_IRQHandler                    USART1_IRQHandler

#endif

/******************** ѡ��USART2����ͨ�� ***********************/
#if USART2_EN == 1

#define DEBUG_USARTx                              USART2
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART2_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_5
#define DEBUG_USARTx_TxRx_GPIO                    GPIOD
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_6
#define DEBUG_USARTx_TxRx_GPIO                    GPIOD

#define DEBUG_USARTx_AFx                          GPIO_AF7_USART2

#define DEBUG_USART_IRQn                          USART2_IRQn
#define DEBUG_USART_IRQHandler                    USART2_IRQHandler

#endif

/******************** ѡ��USART3����ͨ�� ***********************/
#if USART3_EN == 1

#define DEBUG_USARTx                              USART3
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART3_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_10
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_11
#define DEBUG_USARTx_TxRx_GPIO                    GPIOB

#define DEBUG_USARTx_AFx                          GPIO_AF7_USART3

#define DEBUG_USART_IRQn                          USART3_IRQn
#define DEBUG_USART_IRQHandler                    USART3_IRQHandler

#endif

/******************** ѡ��USART6����ͨ�� ***********************/
#if USART6_EN == 1

#define DEBUG_USARTx                              USART6
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_USART6_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOG_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_14
#define DEBUG_USARTx_TxRx_GPIO                    GPIOG
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_9
#define DEBUG_USARTx_TxRx_GPIO                    GPIOG

#define DEBUG_USARTx_AFx                          GPIO_AF7_USART6

#define DEBUG_USART_IRQn                          USART6_IRQn
#define DEBUG_USART_IRQHandler                    USART6_IRQHandler

#endif

/******************** ѡ��UART4����ͨ�� ***********************/
#if UART4_EN == 1

#define DEBUG_USARTx                              UART4
#define DEBUG_USARTx_BAUDRATE                     115200
#define DEBUG_USART_RCC_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()             __HAL_RCC_UART4_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                  GPIO_PIN_10
#define DEBUG_USARTx_TxRx_GPIO                    GPIOC
#define DEBUG_USARTx_Rx_GPIO_PIN                  GPIO_PIN_11
#define DEBUG_USARTx_TxRx_GPIO                    GPIOC

#define DEBUG_USARTx_AFx                          GPIO_AF8_UART4

#define DEBUG_USART_IRQn                          UART4_IRQn
#define DEBUG_USART_IRQHandler                    UART4_IRQHandler

#endif

/* ��չ���� ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart_debug;

/* �������� ------------------------------------------------------------------*/
void MX_DEBUG_UART_Init(void);


#endif /* __BSP_DEBUG_USART_H__ */


/********** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *******END OF FILE******/
