#ifndef _LED_H
#define _LED_H
#include "sys.h"

//LED端口定义
#define LED0(n)		(n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET))
#define LED0_Toggle (HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3)) //LED0输出电平翻转
#define LWIP_RST(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET))

void LED_Init(void); //LED初始化函数
#endif
