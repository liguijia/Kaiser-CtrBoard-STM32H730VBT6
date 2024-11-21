/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2018-09-01
  * ��    ��: ETH�ײ�
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-H7Multiʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/

#include "lwip/err.h"
#include "lwip/netif.h"
/* �궨�� --------------------------------------------------------------------*/

/* ��չ���� ------------------------------------------------------------------*/
extern ETH_HandleTypeDef heth;

/* �������� ------------------------------------------------------------------*/
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void low_level_init(struct netif *netif);
int32_t YS_LAN8720A_Init(void);
void ethernet_link_check_state(struct netif *netif);
void lwip_IT_handle(void);
	
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
