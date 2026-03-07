#include "vofa.h"
#include "stm32f4xx_ll_usart.h"
extern UART_HandleTypeDef huart2;
void sendfloatdata(float data[], uint8_t number)
{
    // 帧尾
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7F};
    // 等待串口空闲
    while(HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
    // 正确发送 float（逐4字节发送，不直接强转）
    for(uint8_t i=0; i<number; i++)
    {
        HAL_UART_Transmit(&huart2, (uint8_t*)&data[i], 4, 100);
    }
    
    // 发送帧尾
    HAL_UART_Transmit(&huart2, tail, 4, 100);
    
    HAL_Delay(10);
}