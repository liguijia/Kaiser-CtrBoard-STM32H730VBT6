#include "LED.h"


//��ʼ��PD3���.��ʹ������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();			//����GPIODʱ��
	__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIObʱ��
    GPIO_Initure.Pin=GPIO_PIN_3; //PD3        LED��
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //��ʼ��GPIOD.3(
	
	GPIO_Initure.Pin=GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);	//PD3��1 
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
}
