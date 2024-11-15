#include "main.h"
#include "fdcan.h"

#include "bsp_can.h"

void BSP_FDCAN_Filter_Init(void)
{
    FDCAN_FilterTypeDef sFilterConfig;

    /* 配置过滤器 */
    sFilterConfig.FilterIndex  = 0;                       // 过滤器索引，范围为0到27
    sFilterConfig.FilterType   = FDCAN_FILTER_MASK;       // 设置过滤器类型为掩码模式
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // 将过滤器结果传输到RX FIFO0
    sFilterConfig.FilterID1    = 0x0000;                  // 过滤器ID1
    sFilterConfig.FilterID2    = 0x0000;                  // 过滤器ID2
    sFilterConfig.IdType       = FDCAN_STANDARD_ID;       // 使用标准标识符

    // 配置过滤器
    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }
}

void BSP_FDCAN_Init(void)
{
    BSP_FDCAN_Filter_Init(); // 初始化FDCAN过滤器

    HAL_FDCAN_Start(&hfdcan1);
    HAL_FDCAN_Start(&hfdcan2);
    HAL_FDCAN_Start(&hfdcan3); // 启动FDCAN总线

    // 激活FDCAN接收FIFO0的新消息通知中断
    // FDCAN_IT_RX_FIFO0_NEW_MESSAGE 表示接收FIFO0有新消息时触发中断
    // 第三个参数为优先级，设置为0表示最高优先级
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 1);
    HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 2);
}

void BSP_FDCAN_Transmit(FDCAN_HandleTypeDef *hfdcan, uint8_t *data)
{
    FDCAN_TxHeaderTypeDef TxHeader; // FDCAN消息头结构体

    // 设置FDCAN消息头的各项参数
    TxHeader.Identifier          = 0x300;              // 消息标识符
    TxHeader.IdType              = FDCAN_STANDARD_ID;  // 使用标准标识符
    TxHeader.TxFrameType         = FDCAN_DATA_FRAME;   // 数据帧类型
    TxHeader.DataLength          = FDCAN_DLC_BYTES_8;  // 数据长度为8字节
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;   // 激活错误状态指示器
    TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;      // 比特率切换关闭
    TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;  // 使用经典CAN格式
    TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS; // 不使用发送事件FIFO
    TxHeader.MessageMarker       = 0;                  // 消息标记为0

    // 向FDCAN的发送FIFO队列中添加消息
    HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, data);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    FDCAN_RxHeaderTypeDef RxHeader; // FDCAN接收消息的标头信息结构体
    uint8_t RxData[8];              // 存储接收数据的数组，最大长度为8字节

    // 检查接收FIFO 0是否有新消息
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
        // 读取接收的消息
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
            Error_Handler(); // 读取消息出错处理
        }
        // 根据消息标识符处理数据
        switch (RxHeader.Identifier) {
            case 0x1FE:
            HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
                // 处理特定标识符的数据
                break;

            default:
                // 处理其他标识符的数据
                break;
        }
    }
}