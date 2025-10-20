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
#define ISR_PRESCALER 170-1
#define ISR_COUNT_PERIOD (TIM_CLK_MHz*1000000/ISR_FREQUENCY/(ISR_PRESCALER + 1)-1)
#define ISR_FREQUENCY 1000
#define TIM_CLK_MHz 170
#define COM_FREQUENCY 10000
#define COM_PRESCALER 0
#define COM_COUNT_PERIOD (TIM_CLK_MHz*1000000/COM_FREQUENCY/(COM_PRESCALER + 1)-1)
#define LED_F0_Pin GPIO_PIN_13
#define LED_F0_GPIO_Port GPIOC
#define LED_F1_Pin GPIO_PIN_14
#define LED_F1_GPIO_Port GPIOC
#define LED_F2_Pin GPIO_PIN_15
#define LED_F2_GPIO_Port GPIOC
#define PRESS_Pin GPIO_PIN_10
#define PRESS_GPIO_Port GPIOG
#define L_TX_Pin GPIO_PIN_2
#define L_TX_GPIO_Port GPIOA
#define L_RX_Pin GPIO_PIN_3
#define L_RX_GPIO_Port GPIOA
#define LED_F3_Pin GPIO_PIN_4
#define LED_F3_GPIO_Port GPIOA
#define LED_F5_Pin GPIO_PIN_5
#define LED_F5_GPIO_Port GPIOA
#define LED_F4_Pin GPIO_PIN_6
#define LED_F4_GPIO_Port GPIOA
#define LED_F6_Pin GPIO_PIN_7
#define LED_F6_GPIO_Port GPIOA
#define LED_F7_Pin GPIO_PIN_4
#define LED_F7_GPIO_Port GPIOC
#define LED_B0_Pin GPIO_PIN_0
#define LED_B0_GPIO_Port GPIOB
#define LED_F8_Pin GPIO_PIN_1
#define LED_F8_GPIO_Port GPIOB
#define LED_B1_Pin GPIO_PIN_2
#define LED_B1_GPIO_Port GPIOB
#define LED_B3_Pin GPIO_PIN_10
#define LED_B3_GPIO_Port GPIOB
#define LED_B2_Pin GPIO_PIN_11
#define LED_B2_GPIO_Port GPIOB
#define LED_B4_Pin GPIO_PIN_12
#define LED_B4_GPIO_Port GPIOB
#define LED_B5_Pin GPIO_PIN_13
#define LED_B5_GPIO_Port GPIOB
#define LED_F10_Pin GPIO_PIN_14
#define LED_F10_GPIO_Port GPIOB
#define LED_F9_Pin GPIO_PIN_15
#define LED_F9_GPIO_Port GPIOB
#define LED_F11_Pin GPIO_PIN_6
#define LED_F11_GPIO_Port GPIOC
#define LED_F12_Pin GPIO_PIN_10
#define LED_F12_GPIO_Port GPIOA
#define LED_F13_Pin GPIO_PIN_10
#define LED_F13_GPIO_Port GPIOC
#define LED_F15_Pin GPIO_PIN_11
#define LED_F15_GPIO_Port GPIOC
#define LED_F14_Pin GPIO_PIN_3
#define LED_F14_GPIO_Port GPIOB
#define LED_F16_Pin GPIO_PIN_4
#define LED_F16_GPIO_Port GPIOB
#define LED_F17_Pin GPIO_PIN_5
#define LED_F17_GPIO_Port GPIOB
#define R_TX_Pin GPIO_PIN_6
#define R_TX_GPIO_Port GPIOB
#define R_RX_Pin GPIO_PIN_7
#define R_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
