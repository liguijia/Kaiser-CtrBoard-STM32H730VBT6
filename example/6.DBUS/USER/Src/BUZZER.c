#include "BUZZER.h"
#include "tim.h"

void BUZZER_0N(uint16_t frequency)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    uint32_t period = (uint32_t)(1e6 / frequency) - 1; // 计算周期
    __HAL_TIM_SET_AUTORELOAD(&htim4, period);

    uint32_t pulse = period / 2; // 默认占空比为50%
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pulse);
}

void updateBuzzerTone(uint16_t voltage)
{
    uint16_t frequency = 2000 - (voltage - 12) * (2000 - 500) / (27 - 8);

    // 限制频率在合理范围内
    if (frequency > 2000) {
        frequency = 2000;
    } else if (frequency < 500) {
        frequency = 500;
    }

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    uint32_t period = (uint32_t)(1e6 / frequency) - 1; // 计算周期
    __HAL_TIM_SET_AUTORELOAD(&htim4, period);

    uint32_t pulse = period / 2; // 默认占空比为50%
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pulse);
}
void BUZZER_Stop()
{
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
}