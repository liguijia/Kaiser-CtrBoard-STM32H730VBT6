#include "LED.h"


//初始化PD3输出.并使能这个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOD时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOb时钟
    GPIO_Initure.Pin=GPIO_PIN_3; //PD3        LED灯
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化GPIOD.3(
	
	GPIO_Initure.Pin=GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);	//PD3置1 
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
}
