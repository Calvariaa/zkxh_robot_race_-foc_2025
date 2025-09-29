//
// Created by Calvaria on 2025/9/20.
//

#ifndef SIMPLEFOC_STM32F431_ENCODER_H
#define SIMPLEFOC_STM32F431_ENCODER_H

#include "spi.h"

/* SPI command for TLE5012 */
#define READ_STATUS        0x8001                        //8000
#define READ_ANGLE_VALUE   0x8021                        //8020
#define READ_SPEED_VALUE   0x8031                        //8030

typedef struct {
  float mechanic_angle;
  float electric_angle;
} encoder_t;

uint16_t read_left_encoder(void);

uint16_t read_right_encoder(void);

#endif //SIMPLEFOC_STM32F431_ENCODER_H
