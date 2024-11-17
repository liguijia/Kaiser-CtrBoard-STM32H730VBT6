/*芯片型号：美光JW824 emmc 8G卡
 *操作类型：块写入，块读取
 *测试时间：2018.5.4--优化读写程序
 *最大容量：7.265625G
 *--------------------------------------------------------*/
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "eMMC.h"
#include "Sram.h"
#include "lan8720.h"
#include "timer.h"
#include "malloc.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "tcp_server_demo.h"

int main(void)
{
	/* Set MPU NOR region */
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				        //初始化HAL库
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
	delay_init(216);
	LED_Init();                     //初始化LED
	FSMC_SRAM_Init();
	TIM3_Init(1000-1,1080-1);        //定时器3初始化，定时器时钟为108M，分频系数为10800-1，
                                    //所以定时器3的频率为108M/1080=100K，自动重装载为1000-1，那么定时器周期就是10ms
	my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMDTCM);		    //初始化DTCM内存池
	while(lwip_comm_init())         //lwip初始化
	{
       delay_ms(200);
	}
	while(1)
	{
		tcp_server_test();
		lwip_periodic_handle();
		delay_us(200);
	}
}

//static void Fill_Buffer(u8 *pBuffer, uint16_t uwBufferLenght)
//{
//  uint16_t tmpIndex = 0;
//  /* Put in global buffer different values */
//  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
//  {
//    pBuffer[tmpIndex] = (u8)(tmpIndex%128);
//  }
//} 
