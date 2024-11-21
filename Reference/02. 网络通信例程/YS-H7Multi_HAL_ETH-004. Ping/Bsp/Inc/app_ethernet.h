/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2018-09-01
  * 功    能: ETH-APP
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-H7Multi使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
#ifndef __APP_ETHERNET_H
#define __APP_ETHERNET_H

/* 包含头文件 ----------------------------------------------------------------*/
#include "lwip/netif.h"
#include "main.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"
#include "lwip/dhcp.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef struct  
{
	uint8_t MAC[6];      //MAC地址
	uint8_t IP[4];       //本机IP地址
	uint8_t NetMask[4]; 	//子网掩码
	uint8_t GateWay[4]; 	//默认网关的IP地址
}Lwip_strut;

/* 宏定义 --------------------------------------------------------------------*/
#define DHCP_OFF                   (uint8_t) 0
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5
   

#define LAN8720A_ADDR            0           //LAN8720地址为0
#define LAN8720A_TIMEOUT     ((uint32_t)500) //LAN8720超时时间

/* LAN8720寄存器 */
#define LAN8720A_BCR      ((uint16_t)0x0000U)
#define LAN8720A_BSR      ((uint16_t)0x0001U)
#define LAN8720A_PHYI1R   ((uint16_t)0x0002U)
#define LAN8720A_PHYI2R   ((uint16_t)0x0003U)
#define LAN8720A_ANAR     ((uint16_t)0x0004U)
#define LAN8720A_ANLPAR   ((uint16_t)0x0005U)
#define LAN8720A_ANER     ((uint16_t)0x0006U)
#define LAN8720A_ANNPTR   ((uint16_t)0x0007U)
#define LAN8720A_ANNPRR   ((uint16_t)0x0008U)
#define LAN8720A_MMDACR   ((uint16_t)0x000DU)
#define LAN8720A_MMDAADR  ((uint16_t)0x000EU)
#define LAN8720A_ENCTR    ((uint16_t)0x0010U)
#define LAN8720A_MCSR     ((uint16_t)0x0011U)
#define LAN8720A_SMR      ((uint16_t)0x0012U)
#define LAN8720A_TPDCR    ((uint16_t)0x0018U)
#define LAN8720A_TCSR     ((uint16_t)0x0019U)
#define LAN8720A_SECR     ((uint16_t)0x001AU)
#define LAN8720A_SCSIR    ((uint16_t)0x001BU)
#define LAN8720A_CLR      ((uint16_t)0x001CU)
#define LAN8720A_ISFR     ((uint16_t)0x001DU)
#define LAN8720A_IMR      ((uint16_t)0x001EU)
#define LAN8720A_PHYSCSR  ((uint16_t)0x001FU)

  
/* LAN8720A BCR寄存器各位描述 */ 
#define LAN8720A_BCR_SOFT_RESET         ((uint16_t)0x8000U)
#define LAN8720A_BCR_LOOPBACK           ((uint16_t)0x4000U)
#define LAN8720A_BCR_SPEED_SELECT       ((uint16_t)0x2000U)
#define LAN8720A_BCR_AUTONEGO_EN        ((uint16_t)0x1000U)
#define LAN8720A_BCR_POWER_DOWN         ((uint16_t)0x0800U)
#define LAN8720A_BCR_ISOLATE            ((uint16_t)0x0400U)
#define LAN8720A_BCR_RESTART_AUTONEGO   ((uint16_t)0x0200U)
#define LAN8720A_BCR_DUPLEX_MODE        ((uint16_t)0x0100U) 

/* LAN8720A的BSR寄存器各位描述 */
#define LAN8720A_BSR_100BASE_T4       ((uint16_t)0x8000U)
#define LAN8720A_BSR_100BASE_TX_FD    ((uint16_t)0x4000U)
#define LAN8720A_BSR_100BASE_TX_HD    ((uint16_t)0x2000U)
#define LAN8720A_BSR_10BASE_T_FD      ((uint16_t)0x1000U)
#define LAN8720A_BSR_10BASE_T_HD      ((uint16_t)0x0800U)
#define LAN8720A_BSR_100BASE_T2_FD    ((uint16_t)0x0400U)
#define LAN8720A_BSR_100BASE_T2_HD    ((uint16_t)0x0200U)
#define LAN8720A_BSR_EXTENDED_STATUS  ((uint16_t)0x0100U)
#define LAN8720A_BSR_AUTONEGO_CPLT    ((uint16_t)0x0020U)
#define LAN8720A_BSR_REMOTE_FAULT     ((uint16_t)0x0010U)
#define LAN8720A_BSR_AUTONEGO_ABILITY ((uint16_t)0x0008U)
#define LAN8720A_BSR_LINK_STATUS      ((uint16_t)0x0004U)
#define LAN8720A_BSR_JABBER_DETECT    ((uint16_t)0x0002U)
#define LAN8720A_BSR_EXTENDED_CAP     ((uint16_t)0x0001U)

/* LAN8720A IMR/ISFR寄存器各位描述 */
#define LAN8720A_INT_7                ((uint16_t)0x0080U)
#define LAN8720A_INT_6                ((uint16_t)0x0040U)
#define LAN8720A_INT_5                ((uint16_t)0x0020U)
#define LAN8720A_INT_4                ((uint16_t)0x0010U)
#define LAN8720A_INT_3                ((uint16_t)0x0008U)
#define LAN8720A_INT_2                ((uint16_t)0x0004U)
#define LAN8720A_INT_1                ((uint16_t)0x0002U)

/* LAN8720A PHYSCSR寄存器各位描述 */
#define LAN8720A_PHYSCSR_AUTONEGO_DONE   ((uint16_t)0x1000U)
#define LAN8720A_PHYSCSR_HCDSPEEDMASK    ((uint16_t)0x001CU)
#define LAN8720A_PHYSCSR_10BT_HD         ((uint16_t)0x0004U)
#define LAN8720A_PHYSCSR_10BT_FD         ((uint16_t)0x0014U)
#define LAN8720A_PHYSCSR_100BTX_HD       ((uint16_t)0x0008U)
#define LAN8720A_PHYSCSR_100BTX_FD       ((uint16_t)0x0018U) 
  
/* LAN8720A状态相关定义 */
#define  LAN8720A_STATUS_READ_ERROR            ((int32_t)-5)
#define  LAN8720A_STATUS_WRITE_ERROR           ((int32_t)-4)
#define  LAN8720A_STATUS_ADDRESS_ERROR         ((int32_t)-3)
#define  LAN8720A_STATUS_RESET_TIMEOUT         ((int32_t)-2)
#define  LAN8720A_STATUS_ERROR                 ((int32_t)-1)
#define  LAN8720A_STATUS_OK                    ((int32_t) 0)
#define  LAN8720A_STATUS_LINK_DOWN             ((int32_t) 1)
#define  LAN8720A_STATUS_100MBITS_FULLDUPLEX   ((int32_t) 2)
#define  LAN8720A_STATUS_100MBITS_HALFDUPLEX   ((int32_t) 3)
#define  LAN8720A_STATUS_10MBITS_FULLDUPLEX    ((int32_t) 4)
#define  LAN8720A_STATUS_10MBITS_HALFDUPLEX    ((int32_t) 5)
#define  LAN8720A_STATUS_AUTONEGO_NOTDONE      ((int32_t) 6)

/* LAN8720A中断标志位定义 */ 
#define  LAN8720A_ENERGYON_IT                   LAN8720A_INT_7
#define  LAN8720A_AUTONEGO_COMPLETE_IT          LAN8720A_INT_6
#define  LAN8720A_REMOTE_FAULT_IT               LAN8720A_INT_5
#define  LAN8720A_LINK_DOWN_IT                  LAN8720A_INT_4
#define  LAN8720A_AUTONEGO_LP_ACK_IT            LAN8720A_INT_3
#define  LAN8720A_PARALLEL_DETECTION_FAULT_IT   LAN8720A_INT_2
#define  LAN8720A_AUTONEGO_PAGE_RECEIVED_IT     LAN8720A_INT_1

/* 扩展变量 ------------------------------------------------------------------*/
extern ETH_HandleTypeDef heth;
extern Lwip_strut Lwip_data;
extern struct netif gnetif;

/* 函数声明 ------------------------------------------------------------------*/

uint32_t ETH_link_check_state(void);
void LAN8720A_StartAutoNego(void);
int32_t ETH_PHY_WriteReg(uint16_t reg,uint16_t value);
int32_t ETH_PHY_ReadReg(uint16_t reg,uint32_t *regval);
uint32_t check_link_state(void);
void Netif_Config(void);
void ethernet_link_status_updated(struct netif *netif);
void Ethernet_Link_Periodic_Handle(struct netif *netif);

#ifdef USE_DHCP
void DHCP_Process(struct netif *netif);
void DHCP_Periodic_Handle(struct netif *netif);
#endif  


#endif /* __APP_ETHERNET_H */


/********** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *******END OF FILE******/

