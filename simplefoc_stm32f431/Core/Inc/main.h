/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define PWM_FREQUENCY 20000
#define PWM_COUNT_PERIOD (TIM_CLK_MHz*1000000/PWM_FREQUENCY/(PWM_PRESCALER + 1)/2-1)
#define PWM_PRESCALER 0
#define MOS_DEADTIME 30
#define TIM_CLK_MHz 170
#define COM_FREQUENCY 10000
#define COM_COUNT_PERIOD (TIM_CLK_MHz*1000000/COM_FREQUENCY/(COM_PRESCALER + 1)-1)
#define COM_PRESCALER 0
#define L_TX_Pin GPIO_PIN_3
#define L_TX_GPIO_Port GPIOA
#define LEFT_LED_Pin GPIO_PIN_2
#define LEFT_LED_GPIO_Port GPIOB
#define R_TX_Pin GPIO_PIN_10
#define R_TX_GPIO_Port GPIOB
#define R_RX_Pin GPIO_PIN_11
#define R_RX_GPIO_Port GPIOB
#define RIGHT_LED_Pin GPIO_PIN_12
#define RIGHT_LED_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_6
#define LED_GPIO_Port GPIOC
#define L_RX_Pin GPIO_PIN_7
#define L_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
