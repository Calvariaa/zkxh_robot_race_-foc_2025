//
// Created by Calvaria on 2025/10/20.
//

#include "isr.h"
#include "fast_foc.h"
#include "encoder.h"

pwm_capture_t L_RX_capture = PWM_CAPTURE_T_INIT(TIM2, TIM_CHANNEL_3, HAL_TIM_ACTIVE_CHANNEL_3);
pwm_capture_t R_RX_capture = PWM_CAPTURE_T_INIT(TIM2, TIM_CHANNEL_4, HAL_TIM_ACTIVE_CHANNEL_4);

// PWM捕获处理函数
void PWM_Capture_Process(const TIM_HandleTypeDef *htim, pwm_capture_t *capture) {
  // if (capture->capture_end_flag == 0) {
    if (capture->capture_state == CAPTURE_RISING) //第一个上升沿
    {
      capture->capture_state = CAPTURE_FALLING;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, capture->Channel, TIM_INPUTCHANNELPOLARITY_FALLING); //设置成下降沿触发
      __HAL_TIM_SetCounter(htim, 0); //清空定时器计数值
      capture->high_val = HAL_TIM_ReadCapturedValue(htim, capture->Channel); //由第一个上升沿设为起始位置
    } else if (capture->capture_state == CAPTURE_FALLING) //第一个下降沿
    {
      capture->capture_state = CAPTURE_RISING_2;
      capture->low_val = HAL_TIM_ReadCapturedValue(htim, capture->Channel); //低电平起始位置
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, capture->Channel, TIM_INPUTCHANNELPOLARITY_RISING); //设置成上升沿触发
    } else if (capture->capture_state == CAPTURE_RISING_2) //第二个上升沿
    {
      capture->capture_state = CAPTURE_RISING;
      capture->high_val = HAL_TIM_ReadCapturedValue(htim, capture->Channel);

      //计算频率
      capture->freq = (float) TIM_CLK_MHz * 1000000 / (RX_PRESCALER + 1) / (capture->high_val + 1);
      //计算占空比
      capture->duty = (float) (capture->low_val + 1) / (capture->high_val + 1);
      // capture->capture_end_flag = 1;
    // }
  }
}
//TIM单通道采集PWM频率+占空比
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == L_RX_capture.Instance) {
    if (htim->Channel == L_RX_capture.ActiveChannel) //通道2
    {
      PWM_Capture_Process(htim, &L_RX_capture);
    }
  }
  if (htim->Instance == R_RX_capture.Instance) {
    if (htim->Channel == R_RX_capture.ActiveChannel) //通道2
    {
      PWM_Capture_Process(htim, &R_RX_capture);
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    HAL_GPIO_TogglePin(LEFT_LED_GPIO_Port, LEFT_LED_Pin);

    float duty = 0;
    duty = L_RX_capture.duty - 0.5f;

    if (duty < -0.3f || duty > 0.3f) duty = 0;
    else if (duty < -0.25f) duty = -0.25f;
    else if (duty > 0.25f) duty = 0.25f;
    duty *= 3.8f;

    foc_control_fast(&motor_left_foc_driver, read_left_encoder(), duty);
  }

  if (htim->Instance == TIM8) {
    HAL_GPIO_TogglePin(RIGHT_LED_GPIO_Port, RIGHT_LED_Pin);

    float duty = 0;
    duty = R_RX_capture.duty - 0.5f;

    if (duty < -0.3f || duty > 0.3f) duty = 0;
    else if (duty < -0.25f) duty = -0.25f;
    else if (duty > 0.25f) duty = 0.25f;
    duty *= 3.8f;

    foc_control_fast(&motor_right_foc_driver, read_right_encoder(), duty);
  }

  if (htim->Instance == TIM6) {
    // PWM_Capture_Read_And_Clear(&L_RX_capture);
    // PWM_Capture_Read_And_Clear(&R_RX_capture);
  }
}
