/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : LED_F0_Pin LED_F1_Pin LED_F2_Pin LED_F7_Pin
                           LED_F11_Pin LED_F13_Pin LED_F15_Pin */
  GPIO_InitStruct.Pin = LED_F0_Pin|LED_F1_Pin|LED_F2_Pin|LED_F7_Pin
                          |LED_F11_Pin|LED_F13_Pin|LED_F15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PRESS_Pin */
  GPIO_InitStruct.Pin = PRESS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PRESS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_F3_Pin LED_F5_Pin LED_F4_Pin LED_F6_Pin
                           LED_F12_Pin */
  GPIO_InitStruct.Pin = LED_F3_Pin|LED_F5_Pin|LED_F4_Pin|LED_F6_Pin
                          |LED_F12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_B0_Pin LED_F8_Pin LED_B1_Pin LED_B3_Pin
                           LED_B2_Pin LED_B4_Pin LED_B5_Pin LED_F10_Pin
                           LED_F9_Pin LED_F14_Pin LED_F16_Pin LED_F17_Pin */
  GPIO_InitStruct.Pin = LED_B0_Pin|LED_F8_Pin|LED_B1_Pin|LED_B3_Pin
                          |LED_B2_Pin|LED_B4_Pin|LED_B5_Pin|LED_F10_Pin
                          |LED_F9_Pin|LED_F14_Pin|LED_F16_Pin|LED_F17_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
