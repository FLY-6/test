#include "ads1292r.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "stm32f411xe.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>
extern SPI_HandleTypeDef hspi1;
float V_REF = 2.4; // 参考电压
float PGA_GAIN = 6.0; // 增益设置
uint8_t ADS_Data_Buf[9];
int32_t ADS_Ch1_Raw;
int32_t ADS_Ch2_Raw;
float ADS_Ch1_Volt;
float ADS_Ch2_Volt;
volatile uint8_t ADS_Data_Ready = 0; // 数据就绪标志
void ads1292r_transmit(uint8_t command)
{
    uint8_t data[2]={command,0x00};
    HAL_GPIO_WritePin(GPIOA, ADS1292_SPI_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, data, 1, 10);
    HAL_GPIO_WritePin(GPIOA, ADS1292_SPI_CS_Pin, GPIO_PIN_SET);
}
/**
 * @brief  读取ADS1292R指定寄存器值（修正版，支持读ID）
 * @param  reg_addr: 寄存器地址（0x00~0x0A）
 * @retval 寄存器值（读ID时正常返回0x92）
 */
uint8_t ADS129x_ReadReg(uint8_t reg_addr)
{
    // 1. 构造正确的RREG指令：高4位=0010（RREG），低4位=寄存器地址
    uint8_t opcode1 = 0x20 | (reg_addr & 0x1F); // 关键！补全RREG标识
    uint8_t opcode2 = 0x00;                     // 只读1个寄存器（n=0）
    uint8_t reg_data = 0xFF;
    uint8_t dummy = 0x00;                       // dummy字节，用于SPI全双工读取

    // 2. 拉低CS，开始SPI通信
    HAL_GPIO_WritePin(GPIOA, ADS1292_SPI_CS_Pin, GPIO_PIN_RESET);
    HAL_Delay(1); // 短暂延时，确保芯片响应

    // 3. 发送读指令（opcode1+opcode2）
    ads1292r_transmit(opcode1);
    HAL_Delay(1);
    ads1292r_transmit(opcode2);
    

    // 4. 发送dummy字节，同时读取寄存器数据（SPI全双工核心）
    HAL_SPI_TransmitReceive(&hspi1, &dummy, &reg_data, 1, 10);

    // 5. 拉高CS，结束通信
    HAL_GPIO_WritePin(GPIOA, ADS1292_SPI_CS_Pin, GPIO_PIN_SET);

    return reg_data;
}
void ads1292r_WriteReg(uint8_t reg, uint8_t data)
{
    uint8_t command[6]={ADS1292R_SDATAC,reg,0x00,data};
    command[1]=0x40 | (reg & 0x1F); // 构造WREG指令：高4位=0100（WREG）+ 低4位=寄存器地址
     HAL_GPIO_WritePin(GPIOA, ADS1292_SPI_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &command[1], 1, 10); // Send register address
    HAL_SPI_Transmit(&hspi1, &command[2], 1, 10);
    HAL_SPI_Transmit(&hspi1, &command[3], 1, 10);
    HAL_GPIO_WritePin(GPIOA, ADS1292_SPI_CS_Pin, GPIO_PIN_SET); 
}

void ads1292r_init(void)
{
    HAL_GPIO_WritePin(ADS1292_RESET_GPIO_Port, ADS1292_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(ADS1292_RESET_GPIO_Port, ADS1292_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    ads1292r_transmit(ADS1292R_SDATAC);
    HAL_Delay(1);
    ads1292r_WriteReg(ADS1292R_REG_CONFIG1,0x02); // Set the sampling rate to 500 SPS
    HAL_Delay(100);
    ads1292r_WriteReg(ADS1292R_REG_CONFIG2,0xA0);
    HAL_Delay(100); // Set the gain to 1
    ads1292r_WriteReg(0x03, 0x00);
    ads1292r_WriteReg(0x06, 0x2c);
    ads1292r_WriteReg(CH1SET,0x00); // Set channel 1 to normal mode
    ads1292r_WriteReg(CH2SET,0x00);
    ads1292r_WriteReg(0x09, 0x02);  // RESP1
    ads1292r_WriteReg(0x0A, 0x02);  // RESP2
    ads1292r_transmit(0x10);
    HAL_Delay(1);
    ads1292r_transmit(0x08);
    HAL_GPIO_WritePin(ADS1292_SPI_CS_GPIO_Port, ADS1292_SPI_CS_Pin, GPIO_PIN_RESET);
    // Set channel 2 to normal mode
    // Initialize the ADS1292R sensor
    // This may include setting up SPI communication, configuring registers, etc.
}
// DRDY 中断回调函数（GPIO_Pin 为 ADS_DRDY_Pin，需与初始化时一致）
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == ADS1292_DRDY_Pin) {  // 仅响应 DRDY 引脚中断
        // 1. 选中器件（拉低 CS 引脚）
        
        
        // 2. 读取 9 字节数据（24位状态字 + 通道1 24位 + 通道2 24位）
        // 注意：SPI 读取时 DIN 需拉低，HAL_SPI_Receive 会自动处理
            ADS_Data_Ready = 1;
        // 7. 取消选中器件（拉高 CS 引脚）
        //HAL_GPIO_WritePin(GPIOA, ADS1292_SPI_CS_Pin, GPIO_PIN_SET);
        // 8. 清除中断标志（HAL 库会自动清除，部分型号需手动清除，根据MCU型号调整）
        __HAL_GPIO_EXTI_CLEAR_IT(ADS1292_DRDY_Pin);
    }
}

