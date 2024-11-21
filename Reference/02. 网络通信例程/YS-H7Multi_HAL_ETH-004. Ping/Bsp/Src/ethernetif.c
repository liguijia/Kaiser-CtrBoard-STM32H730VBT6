/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2018-09-01
  * 功    能: eth-底层驱动
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
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
#include <string.h>
#include "app_ethernet.h"

/* 私有类型定义 --------------------------------------------------------------*/
struct netif lwip_netif;				//定义一个全局的网络接口

/* 私有宏定义 ----------------------------------------------------------------*/
#define IFNAME0 's'
#define IFNAME1 't'

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma location=0x30040000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x30040060
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
#pragma location=0x30040200
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffers */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */
__attribute__((at(0x30040000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x30040060))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
__attribute__((at(0x30040200))) uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffer */
#endif

/* 私有变量 ------------------------------------------------------------------*/

uint32_t current_pbuf_idx = 0;
ETH_HandleTypeDef heth;
ETH_TxPacketConfig TxConfig;

/* 扩展变量 ------------------------------------------------------------------*/
extern __IO uint8_t link_flag;

/* 私有函数原形 --------------------------------------------------------------*/

struct pbuf_custom rx_pbuf[ETH_RX_DESC_CNT];
void pbuf_free_custom(struct pbuf *p);

/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: ETH引脚配置以及初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_ETH_MspInit(ETH_HandleTypeDef* ethHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct={0};
  if(ethHandle->Instance==ETH)
  {
    /* ETH GPIO端口时钟使能 */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    /* E使能ETH外设时钟 */
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();
  

    /**ETH GPIO Configuration    
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PA2     ------> ETH_MDIO
    PA7     ------> ETH_CRS_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PG11     ------> ETH_TX_EN
    PG12     ------> ETH_TXD1
    PG13     ------> ETH_TXD0 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    
    /* 高电平，不使能复位 */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_SET);

    /* 复位引脚配置 */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);    
		
    HAL_NVIC_SetPriority(ETH_IRQn,1,0);         //网络中断优先级应该高一点
    HAL_NVIC_EnableIRQ(ETH_IRQn);   
  }
}

/**
  * 函数功能: ETH引脚反初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef* ethHandle)
{
  if(ethHandle->Instance==ETH)
  {
    /* Disable Peripheral clock */
    __HAL_RCC_ETH1MAC_CLK_DISABLE();
    __HAL_RCC_ETH1TX_CLK_DISABLE();
    __HAL_RCC_ETH1RX_CLK_DISABLE();
  
    /**ETH GPIO Configuration    
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PA2     ------> ETH_MDIO
    PA7     ------> ETH_CRS_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PG11     ------> ETH_TX_EN
    PG12     ------> ETH_TXD1
    PG13     ------> ETH_TXD0 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13);

  }
}

/**
  * 函数功能: 设置网络所使用的0X30040000的ram内存保护
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void NETMPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct={0};

	HAL_MPU_Disable();
	MPU_InitStruct.Enable=MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress=0x30040000;
	MPU_InitStruct.Size=MPU_REGION_SIZE_256B;
	MPU_InitStruct.AccessPermission=MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable=MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable=MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number=MPU_REGION_NUMBER5;
	MPU_InitStruct.TypeExtField=MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable=0x00;
	MPU_InitStruct.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct); 
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
  
}

/**
  * 函数功能: 初始化LAN8720A
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int32_t YS_LAN8720A_Init(void)
{         
	uint32_t timeout=0;
	uint32_t regval=0;
  uint32_t phylink=0;
	int32_t status=LAN8720A_STATUS_OK;
	
	/* MPU保护设置 */ 
	NETMPU_Config();                       
	uint8_t macaddress[6]= {ETH_MAC_ADDR0, ETH_MAC_ADDR1, ETH_MAC_ADDR2, ETH_MAC_ADDR3, ETH_MAC_ADDR4, ETH_MAC_ADDR5};
		
	heth.Instance=ETH;                             /* ETH */
	heth.Init.MACAddr=macaddress;                  /* mac地址 */
	heth.Init.MediaInterface=HAL_ETH_RMII_MODE;    /* RMII接口 */
	heth.Init.RxDesc=DMARxDscrTab;                 /* 发送描述符 */
	heth.Init.TxDesc=DMATxDscrTab;                 /* 接收描述如 */
	heth.Init.RxBuffLen=ETH_MAX_PACKET_SIZE;       /* 接收长度 */
	HAL_ETH_Init(&heth);                           /* 初始化ETH */
	HAL_ETH_SetMDIOClockRange(&heth);
	
	/* LAN8720A软件复位 */
	if(ETH_PHY_WriteReg(LAN8720A_BCR,LAN8720A_BCR_SOFT_RESET)>=0) 
	{
			/* 等待软件复位完成 */     
			if(ETH_PHY_ReadReg(LAN8720A_BCR,&regval)>=0)
			{
					while(regval&LAN8720A_BCR_SOFT_RESET)
					{
							if(ETH_PHY_ReadReg(LAN8720A_BCR,&regval)<0)
							{
									status=LAN8720A_STATUS_READ_ERROR;
									break;
							}
							HAL_Delay(10);
							timeout++;
							if(timeout>=LAN8720A_TIMEOUT) break; /* 超时跳出,5S */
					}   
			}
			else
			{
				status=LAN8720A_STATUS_READ_ERROR;
			}
	}
	else
	{
			status=LAN8720A_STATUS_WRITE_ERROR;
	}

	LAN8720A_StartAutoNego();                /*开启自动协商功能 */
	
	if(status==LAN8720A_STATUS_OK)           /* 如果前面运行正常就延时1s */
	{
		HAL_Delay(1000);                     /* 等待1s */
	}
			
	timeout=0;
	while(ETH_link_check_state()<=LAN8720A_STATUS_LINK_DOWN)  
	{
			HAL_Delay(10);
			timeout++;
			if(timeout>=LAN8720A_TIMEOUT) 
			{
					status=LAN8720A_STATUS_LINK_DOWN;
					break; /* 超时跳出,5S */
			}
	}
	phylink=ETH_link_check_state();
	if(phylink==LAN8720A_STATUS_100MBITS_FULLDUPLEX)
			printf("LAN8720A:100Mb/s FullDuplex\r\n");
	else if(phylink==LAN8720A_STATUS_100MBITS_HALFDUPLEX)
			printf("LAN8720A:100Mb/s HalfDuplex\r\n");
	else if(phylink==LAN8720A_STATUS_10MBITS_FULLDUPLEX)
			printf("LAN8720A:10Mb/s FullDuplex\r\n");
	else if(phylink==LAN8720A_STATUS_10MBITS_HALFDUPLEX)
			printf("LAN8720A:10Mb/s HalfDuplex\r\n");
	return status; 
}

/**
  * 函数功能: 检测是否断线
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ethernet_link_check_state(struct netif *netif)
{
  ETH_MACConfigTypeDef MACConf;
  uint32_t PHYLinkState;
  uint32_t linkchanged = 0, speed = 0, duplex =0;
  
  PHYLinkState = ETH_link_check_state();
  if(netif_is_link_up(netif) && (PHYLinkState <= LAN8720A_STATUS_LINK_DOWN))
  {
		printf("断开网线，需重新连接\n");
    netif_set_down(netif);
    netif_set_link_down(netif);
#if LWIP_NETIF_LINK_CALLBACK
      netif_set_link_callback(&gnetif, ethernet_link_status_updated);
#endif					
  }
  else if(!netif_is_link_up(netif) && (PHYLinkState > LAN8720A_STATUS_LINK_DOWN))
  {
    switch (PHYLinkState)
    {
    case LAN8720A_STATUS_100MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      linkchanged = 1;
      break;
    case LAN8720A_STATUS_100MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      linkchanged = 1;
      break;
    case LAN8720A_STATUS_10MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      linkchanged = 1;
      break;
    case LAN8720A_STATUS_10MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      linkchanged = 1;
      break;
    default:
      break;      
    }
    
    if(linkchanged)
    {
      /* Get MAC Config MAC */
      HAL_ETH_GetMACConfig(&heth, &MACConf); 
      MACConf.DuplexMode = duplex;
      MACConf.Speed = speed;
      HAL_ETH_SetMACConfig(&heth, &MACConf);
      			
      netif_set_up(netif);
      netif_set_link_up(netif);
    }
  }
}


/**
  * 函数功能: 方便中断处理函数调用
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void lwip_IT_handle(void)
{
    //从网络缓冲区中读取接收到的数据包并将其发送给LWIP处理 
    ethernetif_input(&gnetif);
}

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH) 
*******************************************************************************/
/**
 * @brief In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
void low_level_init(struct netif *netif)
{ 
	uint32_t idx = 0;
  ETH_MACConfigTypeDef MACConf;
  uint32_t PHYLinkState;
  uint32_t speed = 0, duplex =0;
	
	netif->hwaddr_len=ETHARP_HWADDR_LEN;  /* 设置MAC地址长度,为6个字节 */
	/* 初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复 */
	netif->hwaddr[0] =  0x02;
	netif->hwaddr[1] =  0x00;
	netif->hwaddr[2] =  0x00;
	netif->hwaddr[3] =  0x00;
	netif->hwaddr[4] =  0x00;
	netif->hwaddr[5] =  0x00;
	netif->mtu=ETH_MAX_PAYLOAD;/* 最大允许传输单元,允许该网卡广播和ARP功能 */

	netif->flags|=NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP;
	for(idx=0;idx<ETH_RX_DESC_CNT;idx ++)
	{
			HAL_ETH_DescAssignMemory(&heth,idx,Rx_Buff[idx],NULL);
			rx_pbuf[idx].custom_free_function=pbuf_free_custom;
	}

	memset(&TxConfig,0,sizeof(ETH_TxPacketConfig));
	TxConfig.Attributes=ETH_TX_PACKETS_FEATURES_CSUM|ETH_TX_PACKETS_FEATURES_CRCPAD;
	TxConfig.ChecksumCtrl=ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	TxConfig.CRCPadCtrl=ETH_CRC_PAD_INSERT;
	
	
  PHYLinkState = ETH_link_check_state();

	switch (PHYLinkState)
	{
	case LAN8720A_STATUS_100MBITS_FULLDUPLEX:
		duplex = ETH_FULLDUPLEX_MODE;
		speed = ETH_SPEED_100M;
		break;
	case LAN8720A_STATUS_100MBITS_HALFDUPLEX:
		duplex = ETH_HALFDUPLEX_MODE;
		speed = ETH_SPEED_100M;

		break;
	case LAN8720A_STATUS_10MBITS_FULLDUPLEX:
		duplex = ETH_FULLDUPLEX_MODE;
		speed = ETH_SPEED_10M;

		break;
	case LAN8720A_STATUS_10MBITS_HALFDUPLEX:
		duplex = ETH_HALFDUPLEX_MODE;
		speed = ETH_SPEED_10M;

		break;
	default:
		break;      
	}
	/* Get MAC Config MAC */
	HAL_ETH_GetMACConfig(&heth, &MACConf); 
	MACConf.DuplexMode = duplex;
	MACConf.Speed = speed;
	
	HAL_ETH_SetMACConfig(&heth, &MACConf);			
	HAL_ETH_Start_IT(&heth);	
	
	netif_set_up(netif);
	netif_set_link_up(netif);
	
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
   uint32_t i=0, framelen = 0;
  struct pbuf *q;
  err_t errval = ERR_OK;
  ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT];
  
  for(q = p; q != NULL; q = q->next)
  {
    if(i >= ETH_TX_DESC_CNT)	
      return ERR_IF;
    
    Txbuffer[i].buffer = q->payload;
    Txbuffer[i].len = q->len;
    framelen += q->len;
    
    if(i>0)
    {
      Txbuffer[i-1].next = &Txbuffer[i];
    }
    
    if(q->next == NULL)
    {
      Txbuffer[i].next = NULL;
    }
    
    i++;
  }

  TxConfig.Length = framelen;
  TxConfig.TxBuffer = Txbuffer;
  
  /* Clean and Invalidate data cache */
  SCB_CleanInvalidateDCache();
  
  HAL_ETH_Transmit(&heth, &TxConfig, 0);
  
  return errval;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
   */
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p = NULL;
  ETH_BufferTypeDef RxBuff={0};
  uint32_t framelength = 0;
  
  
  if (HAL_ETH_IsRxDataAvailable(&heth))
  {
    /* Clean and Invalidate data cache */
    SCB_CleanInvalidateDCache();
  
    HAL_ETH_GetRxDataBuffer(&heth, &RxBuff);
    HAL_ETH_GetRxDataLength(&heth, &framelength);
    
    p = pbuf_alloced_custom(PBUF_RAW, framelength, PBUF_POOL, &rx_pbuf[current_pbuf_idx], RxBuff.buffer, framelength);
    
    if(current_pbuf_idx < (ETH_RX_DESC_CNT -1))
    {
      current_pbuf_idx++;
    }
    else
    {
      current_pbuf_idx = 0;
    }
    
    return p;
  }
  else
  {
    return NULL;
  }
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(struct netif *netif)
{
  err_t err;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
    
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
    
  /* entry point to the LwIP stack */
  err = netif->input(p, netif);
    
  if (err != ERR_OK)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
    pbuf_free(p);
    p = NULL;    
  }
  HAL_ETH_BuildRxDescriptors(&heth);
  
}

#if !LWIP_ARP
/**
 * This function has to be completed by user in case of ARP OFF.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if ...
 */
static err_t low_level_output_arp_off(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{  
  err_t errval;
  errval = ERR_OK;
    
/* USER CODE BEGIN 5 */ 
    
/* USER CODE END 5 */  
    
  return errval;
  
}
#endif /* LWIP_ARP */ 

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));
  
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */

#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP
  netif->output = etharp_output;
#else
  /* The user should write ist own code in low_level_output_arp_off function */
  netif->output = low_level_output_arp_off;
#endif /* LWIP_ARP */
#endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */
 
#if LWIP_IPV6
  netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

/**
  * @brief  Custom Rx pbuf free callback
  * @param  pbuf: pbuf to be freed
  * @retval None
  */
void pbuf_free_custom(struct pbuf *p)
{
  if(p!=NULL)
  {
    p->flags = 0;
    p->next = NULL;
    p->len = p->tot_len = 0;
    p->ref = 0;
    p->payload = NULL;
  }
}

/* USER CODE BEGIN 6 */

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Current Time value
*/
u32_t sys_jiffies(void)
{
  return HAL_GetTick();
}

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Current Time value
*/
u32_t sys_now(void)
{
  return HAL_GetTick();
}

/* USER CODE END 6 */


/* USER CODE BEGIN 8 */

/* USER CODE END 8 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

