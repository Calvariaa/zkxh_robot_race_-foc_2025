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
  volatile TIM_TypeDef *Instance;
  volatile uint32_t Channel;
  volatile HAL_TIM_ActiveChannel ActiveChannel;
  volatile float freq;
  volatile float duty;
  volatile int capture_end_flag;
  volatile uint32_t high_val;
  volatile uint32_t low_val;
  volatile capture_state_t capture_state;
} pwm_capture_t;

#define PWM_CAPTURE_T_INIT(_Instance, _Channel, _ActiveChannel) \
{ \
.Instance = _Instance, \
.Channel = _Channel, \
.ActiveChannel = _ActiveChannel, \
.freq = 0.0f, \
.duty = 0.0f, \
.capture_end_flag = 0, \
.high_val = 0, \
.low_val = 0, \
.capture_state = CAPTURE_RISING \
}

extern pwm_capture_t L_RX_capture;
extern pwm_capture_t R_RX_capture;

#endif //MAINBOARD_STM32F431_ISR_H