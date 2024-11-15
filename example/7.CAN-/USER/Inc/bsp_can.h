#pragma once

#include <stdint.h>

extern void BSP_FDCAN_Filter_Init(void);
extern void BSP_FDCAN_Init(void);
extern void BSP_FDCAN_Transmit(FDCAN_HandleTypeDef *hfdcan, uint8_t *data);