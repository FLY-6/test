/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADS1292_SPI_CS_Pin GPIO_PIN_4
#define ADS1292_SPI_CS_GPIO_Port GPIOA
#define ADS1292_GPIO2_Pin GPIO_PIN_0
#define ADS1292_GPIO2_GPIO_Port GPIOB
#define ADS1292R_GPIO1_Pin GPIO_PIN_1
#define ADS1292R_GPIO1_GPIO_Port GPIOB
#define ADS1292_CLKSEL_Pin GPIO_PIN_2
#define ADS1292_CLKSEL_GPIO_Port GPIOB
#define ADS1292_START_Pin GPIO_PIN_11
#define ADS1292_START_GPIO_Port GPIOA
#define ADS1292_RESET_Pin GPIO_PIN_12
#define ADS1292_RESET_GPIO_Port GPIOA
#define ADS1292_DRDY_Pin GPIO_PIN_15
#define ADS1292_DRDY_GPIO_Port GPIOA
#define ADS1292_DRDY_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
