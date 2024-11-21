/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2018-09-01
  * ��    ��: ����ͨ�Ź���ʵ��
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
#include "bsp_debug_usart.h"
#include <stdio.h>

/* ˽�����Ͷ��� --------------------------------------------------------------*/

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husart_debug={0};

/* ��չ���� ------------------------------------------------------------------*/

/* ˽�к���ԭ�� --------------------------------------------------------------*/

/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ���ش������ų�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��ʼ���������ţ���������HAL�����
  */
static void YS_UARTx_MspInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* USART ʱ��ʹ�� */
  DEBUG_USART_RCC_CLK_ENABLE();
  /* ���� ʱ��ʹ�� */
  DEBUG_USARTx_GPIO_ClK_ENABLE();
  
  GPIO_InitStruct.Pin = DEBUG_USARTx_Tx_GPIO_PIN |\
                        DEBUG_USARTx_Rx_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USARTx_AFx;
  HAL_GPIO_Init(DEBUG_USARTx_TxRx_GPIO, &GPIO_InitStruct);

  /* USART �ж����ȼ�����*/
  HAL_NVIC_SetPriority(DEBUG_USART_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQn);

}

/**
  * ��������: ���ش���ͨ�ų�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��ʼ������ͨ�Ź���
  */
void MX_DEBUG_UART_Init(void)
{
  /* ����ͨ��GPIO��ʼ�� */
  YS_UARTx_MspInit();
  /* ����ͨ�Ź������� */
  husart_debug.Instance = DEBUG_USARTx;
  husart_debug.Init.BaudRate   = DEBUG_USARTx_BAUDRATE;
  husart_debug.Init.WordLength = UART_WORDLENGTH_8B;
  husart_debug.Init.StopBits   = UART_STOPBITS_1;
  husart_debug.Init.Parity     = UART_PARITY_NONE;
  husart_debug.Init.Mode       = UART_MODE_TX_RX;
  husart_debug.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  husart_debug.Init.OverSampling    = UART_OVERSAMPLING_16;
  husart_debug.Init.OneBitSampling  = UART_ONE_BIT_SAMPLE_DISABLE;
  husart_debug.Init.Prescaler       = UART_PRESCALER_DIV1;
  husart_debug.Init.FIFOMode        = UART_FIFOMODE_DISABLE;
  husart_debug.Init.TXFIFOThreshold = UART_TXFIFO_THRESHOLD_1_8;
  husart_debug.Init.RXFIFOThreshold = UART_RXFIFO_THRESHOLD_1_8;
  husart_debug.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&husart_debug);

}

/**
  * ��������: �ض���c�⺯��printf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&husart_debug, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart_debug,&ch, 1, 0xffff);
  return ch;
}


/********** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *******END OF FILE******/
