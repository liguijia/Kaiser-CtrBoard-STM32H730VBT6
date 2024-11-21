/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2018-09-01
  * 功    能: LwIP协议配置
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
#include "stm32h7xx_hal.h"
#include "lwip/dhcp.h"
#include "app_ethernet.h"

/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
#ifdef USE_DHCP
#define MAX_DHCP_TRIES  4
uint32_t DHCPfineTimer = 0;
__IO uint8_t DHCP_state = DHCP_OFF;

#endif
Lwip_strut Lwip_data;
struct netif gnetif;
uint32_t EthernetLinkTimer;

/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 读取PHY寄存器值
  * 输入参数: reg：要读取的寄存器地址
  * 返 回 值: 0：读取成功，-1：读取失败
  * 说    明: 无
  */
int32_t ETH_PHY_ReadReg(uint16_t reg,uint32_t *regval)
{
    if(HAL_ETH_ReadPHYRegister(&heth,LAN8720A_ADDR,reg,regval)!=HAL_OK)
        return -1;
    return 0;
}

/**
  * 函数功能: 向LAN8720A指定寄存器写入值
  * 输入参数: reg：要读取的寄存器地址 ，value:要写入的值
  * 返 回 值: 0：写入正常，-1：写入失败
  * 说    明: 无
  */
int32_t ETH_PHY_WriteReg(uint16_t reg,uint16_t value)
{
    uint32_t temp=value;
    if(HAL_ETH_WritePHYRegister(&heth,LAN8720A_ADDR,reg,temp)!=HAL_OK)
        return -1;
    return 0;
}

/**
  * 函数功能: 开启LAN8720A的自协商功能
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void LAN8720A_StartAutoNego(void)
{
    uint32_t readval=0;
    ETH_PHY_ReadReg(LAN8720A_BCR,&readval);
    readval|=LAN8720A_BCR_AUTONEGO_EN;
    ETH_PHY_WriteReg(LAN8720A_BCR,readval);
}

/**
  * 函数功能: 获取LAN8720A的连接状态
  * 输入参数: 无
  * 返 回 值: LAN8720A_STATUS_LINK_DOWN           ： 连接断开        
  * 说    明: 无
  */
uint32_t check_link_state(void)
{
	
	uint32_t readval=0;
	
	/* 读取两遍，确保读取正确！！！ */
	ETH_PHY_ReadReg(LAN8720A_BSR,&readval);
	ETH_PHY_ReadReg(LAN8720A_BSR,&readval);
	
	/* 获取连接状态(硬件，网线的连接，不是TCP、UDP等软件连接！) */
	if((readval&LAN8720A_BSR_LINK_STATUS)==0)
	{

		return LAN8720A_STATUS_LINK_DOWN;
	}
  else 	
	return LAN8720A_STATUS_OK;
}	

/**
  * 函数功能: 获取LAN8720A的连接状态
  * 输入参数: 无
  * 返 回 值: LAN8720A_STATUS_LINK_DOWN           ： 连接断开 
              LAN8720A_STATUS_AUTONEGO_NOTDONE    ： 自动协商完成
              LAN8720A_STATUS_100MBITS_FULLDUPLEX ： 100M全双工
              LAN8720A_STATUS_100MBITS_HALFDUPLEX ： 100M半双工
              LAN8720A_STATUS_10MBITS_FULLDUPLEX  ： 10M全双工
              LAN8720A_STATUS_10MBITS_HALFDUPLEX  ： 10M半双工              
  * 说    明: 无
  */
uint32_t ETH_link_check_state(void)
{
    uint32_t readval=0;
    
    /* 读取两遍，确保读取正确！！！ */
    ETH_PHY_ReadReg(LAN8720A_BSR,&readval);
    ETH_PHY_ReadReg(LAN8720A_BSR,&readval);
    /* 获取连接状态(硬件，网线的连接，不是TCP、UDP等软件连接！) */
    if((readval&LAN8720A_BSR_LINK_STATUS)==0)
    {
      return LAN8720A_STATUS_LINK_DOWN;
    }
        
    /* 获取自动协商状态 */
    ETH_PHY_ReadReg(LAN8720A_BCR,&readval);
    if((readval&LAN8720A_BCR_AUTONEGO_EN)!=LAN8720A_BCR_AUTONEGO_EN)  /* 未使能自动协商 */
    {
        if(((readval&LAN8720A_BCR_SPEED_SELECT)==LAN8720A_BCR_SPEED_SELECT)&&
                ((readval&LAN8720A_BCR_DUPLEX_MODE)==LAN8720A_BCR_DUPLEX_MODE)) 
            return LAN8720A_STATUS_100MBITS_FULLDUPLEX;
        else if((readval&LAN8720A_BCR_SPEED_SELECT)==LAN8720A_BCR_SPEED_SELECT)
            return LAN8720A_STATUS_100MBITS_HALFDUPLEX;
        else if((readval&LAN8720A_BCR_DUPLEX_MODE)==LAN8720A_BCR_DUPLEX_MODE)
            return LAN8720A_STATUS_10MBITS_FULLDUPLEX;
        else
            return LAN8720A_STATUS_10MBITS_HALFDUPLEX;
    }
    else                                                             /* 使能了自动协商 */
    {
        ETH_PHY_ReadReg(LAN8720A_PHYSCSR,&readval);
        if((readval&LAN8720A_PHYSCSR_AUTONEGO_DONE)==0)
            return LAN8720A_STATUS_AUTONEGO_NOTDONE;
        if((readval&LAN8720A_PHYSCSR_HCDSPEEDMASK)==LAN8720A_PHYSCSR_100BTX_FD)
            return LAN8720A_STATUS_100MBITS_FULLDUPLEX;
        else if ((readval&LAN8720A_PHYSCSR_HCDSPEEDMASK)==LAN8720A_PHYSCSR_100BTX_HD)
            return LAN8720A_STATUS_100MBITS_HALFDUPLEX;
        else if ((readval&LAN8720A_PHYSCSR_HCDSPEEDMASK)==LAN8720A_PHYSCSR_10BT_FD)
            return LAN8720A_STATUS_10MBITS_FULLDUPLEX;
        else
            return LAN8720A_STATUS_10MBITS_HALFDUPLEX;         
    }
}

/**
  * 函数功能: ETH中断服务函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ETH_IRQHandler(void)
{
  lwip_IT_handle();

	__HAL_ETH_DMA_CLEAR_IT(&heth,ETH_DMA_NORMAL_IT);  //清除DMA中断标志位
	__HAL_ETH_DMA_CLEAR_IT(&heth,ETH_DMA_RX_IT);      //清除DMA接收中断标志位
	__HAL_ETH_DMA_CLEAR_IT(&heth,ETH_DMA_TX_IT);      //清除DMA接收中断标志位
}


#ifdef USE_DHCP
/**
  * 函数功能: DHCP获取函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void DHCP_Process(struct netif *netif)
{
  uint32_t ipaddr=0,netmask=0,gw=0;
  struct dhcp *dhcp;   

  uint8_t iptxt[20];
  
  switch (DHCP_state)
  {
    case DHCP_START:
    {     
      DHCP_state = DHCP_WAIT_ADDRESS;
      dhcp_start(netif);
      printf (" State: Looking for DHCP server ...\n");
    }
    break;
    
  case DHCP_WAIT_ADDRESS:
    {
      printf ("DHCP_state=%d\n",DHCP_state);
      if (dhcp_supplied_address(netif)) 
      {
        DHCP_state = DHCP_ADDRESS_ASSIGNED; 
        /* 获取IP地址 */
        ipaddr=netif->ip_addr.addr;
        
        Lwip_data.MAC[3]=ETH_MAC_ADDR3;
        Lwip_data.MAC[2]=ETH_MAC_ADDR2;
        Lwip_data.MAC[1]=ETH_MAC_ADDR1;
        Lwip_data.MAC[0]=ETH_MAC_ADDR0;
        printf (" MAC  地址:         %d %d %d %d\n", Lwip_data.MAC[0],Lwip_data.MAC[1],Lwip_data.MAC[2],Lwip_data.MAC[3]);  
               
        Lwip_data.IP[3]=(uint8_t)(ipaddr>>24);
        Lwip_data.IP[2]=(uint8_t)(ipaddr>>16);
        Lwip_data.IP[1]=(uint8_t)(ipaddr>>8);
        Lwip_data.IP[0]=(uint8_t)(ipaddr);
        printf (" DHCP GET IP:      %d %d %d %d\n", Lwip_data.IP[0],Lwip_data.IP[1],Lwip_data.IP[2],Lwip_data.IP[3]);  
        /* 获取子网掩码地址 */
        netmask=netif->netmask.addr;
        Lwip_data.NetMask[3]=(uint8_t)(netmask>>24);
        Lwip_data.NetMask[2]=(uint8_t)(netmask>>16);
        Lwip_data.NetMask[1]=(uint8_t)(netmask>>8);
        Lwip_data.NetMask[0]=(uint8_t)(netmask);
        printf (" DHCP GET 子网掩码: %d %d %d %d\n", Lwip_data.NetMask[0],Lwip_data.NetMask[1],Lwip_data.NetMask[2],Lwip_data.NetMask[3]);  
        /* 获取默认网关地址 */
        gw=netif->gw.addr;
        Lwip_data.GateWay[3]=(uint8_t)(gw>>24);
        Lwip_data.GateWay[2]=(uint8_t)(gw>>16);
        Lwip_data.GateWay[1]=(uint8_t)(gw>>8);
        Lwip_data.GateWay[0]=(uint8_t)(gw);
        printf (" DHCP GET 默认网关: %d %d %d %d\n", Lwip_data.GateWay[0],Lwip_data.GateWay[1],Lwip_data.GateWay[2],Lwip_data.GateWay[3]);  
               
//        sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));        
//        printf (" IP address assigned by a DHCP server: %s\n", iptxt);

      }
      else
      {
        dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    
        /* DHCP timeout */
        if (dhcp->tries > MAX_DHCP_TRIES)
        {
          DHCP_state = DHCP_TIMEOUT;
          
          /* Stop DHCP */
          dhcp_stop(netif);
          
          /* Static address used */
          IP_ADDR4(&(netif->ip_addr), IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
          IP_ADDR4(&(netif->netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
          IP_ADDR4(&(netif->gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
      
          sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
          printf ("DHCP Timeout !! \n");
          printf ("Static IP address: %s\n", iptxt); 
				
        }
      }
    }
    break;
  case DHCP_LINK_DOWN:
    {
			printf("关闭DHCP\n");
      /* Stop DHCP */
      dhcp_stop(netif);
      DHCP_state = DHCP_OFF; 
    }
    break;
  default: break;
  }
}

/**
  * 函数功能: DHCP轮询
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void DHCP_Periodic_Handle(struct netif *netif)
{  
  /* Fine DHCP periodic process every 500ms */
  if (HAL_GetTick() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  HAL_GetTick();
		dhcp_fine_tmr();
    /* process DHCP state machine */
    DHCP_Process(netif);    
  }
}
#endif


#if LWIP_NETIF_LINK_CALLBACK
/**
  * 函数功能: 网络链接检测
  * 输入参数: netif
  * 返 回 值: 无
  * 说    明: 无
  */
void Ethernet_Link_Periodic_Handle(struct netif *netif)
{
  /* Ethernet Link every 100ms */
  if (HAL_GetTick() - EthernetLinkTimer >= 100)
  {
    EthernetLinkTimer = HAL_GetTick();
    ethernet_link_check_state(netif);
  }
}
#endif

/**
  * 函数功能: 配置网络接口
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void Netif_Config(void)
{
  uint8_t i=0;
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  
  /* 初始化LAN8720,如果失败的话就重试10次 */
  while(YS_LAN8720A_Init())		        
  {
      i++;
      if(i>10) 
      {
				i=0;
				printf("LAN8720初始化失败\n");		
			} /* LAN8720初始化失败 */
  }		 
  /* 初始化LWIP内核 */
	lwip_init();					 	
  
#ifdef USE_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP4_ADDR(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP4_ADDR(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP4_ADDR(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
  
	printf("静态IP地址.......%d.%d.%d.%d\r\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	printf("子网掩码.....  ..%d.%d.%d.%d\r\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
	printf("默认网关....... .%d.%d.%d.%d\r\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
	
#endif /* USE_DHCP */
  
  /* 添加网络接口 */    
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /* 注册默认网络接口 */
  netif_set_default(&gnetif);
	
  ethernet_link_status_updated(&gnetif);

#if LWIP_NETIF_LINK_CALLBACK
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);
#endif

}

void ethernet_link_status_updated(struct netif *netif)
{
  if (netif_is_link_up(netif))
  {
    printf("成功连接网卡\n");
    /* 当连接网卡成功时，调用此函数 */
    netif_set_up(&gnetif);		
#ifdef USE_DHCP
    /* 标志位设置为 DHCP_START*/
    DHCP_state = DHCP_START;
	  dhcp_start(&gnetif);
#else
    uint8_t iptxt[20];
    sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&gnetif.ip_addr));
    printf ("Static IP address: %s\n", iptxt);

#endif /* USE_DHCP */
  }
  else
  {
    /* 当连接网卡失败时，调用此函数 */
    netif_set_down(&gnetif);		
#ifdef USE_DHCP
    /* 标志位设置为 DHCP_LINK_DOWN */
    DHCP_state = DHCP_LINK_DOWN;
#endif  /* USE_DHCP */
    printf ("The network cable is not connected \n");

  }
}


/********** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *******END OF FILE******/
