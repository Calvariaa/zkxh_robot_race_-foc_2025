//
// Created by Calvaria on 2025/10/20.
//

#include "isr.h"
#include "control.h"
// #include "mpu6050.h"
#include "i2c.h"
#include "stdio.h"
// #include "data.h"
#include "gray_sensor.h"
#include "icm42688.h"

pwm_capture_t L_RX_capture = PWM_CAPTURE_T_INIT(TIM15, TIM_CHANNEL_2, HAL_TIM_ACTIVE_CHANNEL_2);
pwm_capture_t R_RX_capture = PWM_CAPTURE_T_INIT(TIM15, TIM_CHANNEL_1, HAL_TIM_ACTIVE_CHANNEL_1);

speed_struct_t speed_struct = {0};

int clip(int x, int low, int up) {
  return x > up
           ? up
           : x < low
               ? low
               : x;
}

// PWM捕获处理函数
void PWM_Capture_Process(const TIM_HandleTypeDef *htim, pwm_capture_t *capture) {
  if (capture->capture_end_flag == 0) {
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
      capture->capture_end_flag = 1;
    }
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

int32_t count = 0;
int32_t last_count = 0;

uint16_t pit_time_2ms = 0, pit_time_10ms = 0;
uint64_t total_time_10ms = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  // 1ms tim interrupt
  if (htim->Instance == TIM6) {
    pit_time_2ms++;
    pit_time_10ms++;

    if (pit_time_2ms == 1) {
      pit_time_2ms = 0;

      if (L_RX_capture.capture_end_flag == 1) {
        speed_struct.left = (L_RX_capture.duty * (L_RX_capture.freq / 500.f) * 2 - 0.9958f);
        L_RX_capture.capture_end_flag = 0;
      }
      if (R_RX_capture.capture_end_flag == 1) {
        speed_struct.right = -(R_RX_capture.duty * (R_RX_capture.freq / 500.f) * 2 - 0.9958f);
        R_RX_capture.capture_end_flag = 0;
      }

      read_gray_sensor();

      // 读取陀螺仪数据
      // MPU6050_Read_Gyro(&hi2c2, &MPU6050);
      // ICM42688_ReadGyro(&hi2c2, gyro);
      ICM42688_ReadGyro_DMA(gyro);


      turn_error = (int16_t) calc_left_right_error_enhanced();

      int16_t error = clip(gyro[2] + turn_error * 500, -4000, 4000);

      int16_t speed = 2000;


      int16_t speed_left = speed - (int16_t) (speed_struct.left * 20000);
      int16_t speed_right = speed - (int16_t) (speed_struct.right * 20000);

      float turn_speed_error = speed_struct.left - speed_struct.right;

      int16_t turn_left = error + (int16_t) (turn_speed_error * 8000);
      int16_t turn_right = error + (int16_t) (turn_speed_error * 8000);

      int16_t left_period = clip((turn_left + speed_left + 20000) * TX_COUNT_PERIOD / 40000, 0,
                                 TX_COUNT_PERIOD - 1);
      int16_t right_period = clip((turn_right - speed_right + 20000) * TX_COUNT_PERIOD / 40000, 0,
                                  TX_COUNT_PERIOD - 1);

      if (control_state != STOP) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left_period); // L_TX
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, right_period); // R_TX
      } else {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,
                              TX_COUNT_PERIOD / 2); // L_TX
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,
                              TX_COUNT_PERIOD / 2); // R_TX
      }

      // direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
      count = (int32_t) (__HAL_TIM_GET_COUNTER(&htim2) + 2) >> 2;
    }
    if (pit_time_10ms == 5) {
      pit_time_10ms = 0;

      key_scan();
      total_time_10ms++;
    }
  }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  // for (int i = 0; i < 3; i++)
  if (hi2c->Instance == hi2c2.Instance)
    // ICM42688_ReadGyro_DMA(gyro);
    HAL_I2C_Mem_Read_DMA(&hi2c2, ICM42688_ADDRESS << 1, GYRO_DATA_X1, 1, dma_gyro_data, 6);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
  if (hi2c->Instance == hi2c2.Instance) {
    // 释放I2C端口为GPIO
    HAL_I2C_DeInit(&hi2c2);
    // 强制拉高SCL和SDA
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);
    // 重新初始化I2C控制器
    HAL_I2C_Init(&hi2c2);
  }
}
