//
// Created by Calvaria on 2025/10/20.
//

#ifndef MAINBOARD_STM32F431_ISR_H
#define MAINBOARD_STM32F431_ISR_H

#include "stm32g4xx_hal.h"
#include "tim.h"

typedef enum {
  CAPTURE_RISING = 0,
  CAPTURE_FALLING = 1,
  CAPTURE_RISING_2 = 2
} capture_state_t;

typedef struct {
  TIM_TypeDef *Instance;
  uint32_t Channel;
  HAL_TIM_ActiveChannel ActiveChannel;

  volatile uint32_t high_val;
  volatile uint32_t low_val;

  volatile capture_state_t capture_state;
  volatile int capture_end_flag;

  volatile uint32_t high_val_sum;
  volatile uint32_t low_val_sum;
  volatile uint32_t count;
  volatile float freq;
  volatile float duty;
} pwm_capture_t;

typedef struct {
  float left;
  float right;
} speed_struct_t;

extern speed_struct_t speed_struct;

#define PWM_CAPTURE_T_INIT(_Instance, _Channel, _ActiveChannel) \
{ \
.Instance = _Instance, \
.Channel = _Channel, \
.ActiveChannel = _ActiveChannel, \
.high_val = 0, \
.low_val = 0, \
.capture_state = CAPTURE_RISING, \
.capture_end_flag = 0, \
.high_val_sum = 0, \
.low_val_sum = 0, \
.count = 0, \
.freq = 0.0f, \
.duty = 0.0f \
}

extern pwm_capture_t L_RX_capture;
extern pwm_capture_t R_RX_capture;

#endif //MAINBOARD_STM32F431_ISR_H