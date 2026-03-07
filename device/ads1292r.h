#include "main.h"
void ads1292r_init(void);
uint8_t ADS129x_ReadReg(uint8_t reg_addr);
void ads1292r_transmit(uint8_t command);
extern float ADS_Ch1_Volt;
extern float ADS_Ch2_Volt;
extern uint8_t ADS_Data_Buf[9];
extern int32_t ADS_Ch1_Raw;
extern int32_t ADS_Ch2_Raw;
extern float V_REF;
extern float PGA_GAIN;
extern volatile uint8_t ADS_Data_Ready;
#define ADS1292R_SDATAC 0x11
#define ADS1292R_REG_CONFIG1 0x01
#define ADS1292R_REG_CONFIG2 0x02
#define CH1SET 0x04
#define CH2SET 0x05


