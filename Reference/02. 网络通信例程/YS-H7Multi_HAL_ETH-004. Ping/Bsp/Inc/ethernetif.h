/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2018-09-01
  * 功    能: ETH底层
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-H7Multi使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

/* 包含头文件 ----------------------------------------------------------------*/

#include "lwip/err.h"
#include "lwip/netif.h"
/* 宏定义 --------------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/
extern ETH_HandleTypeDef heth;

/* 函数声明 ------------------------------------------------------------------*/
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void low_level_init(struct netif *netif);
int32_t YS_LAN8720A_Init(void);
void ethernet_link_check_state(struct netif *netif);
void lwip_IT_handle(void);
	
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
