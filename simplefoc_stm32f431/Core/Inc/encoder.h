//
// Created by Calvaria on 2025/9/20.
//

#ifndef SIMPLEFOC_STM32F431_ENCODER_H
#define SIMPLEFOC_STM32F431_ENCODER_H

#include "spi.h"

uint16_t read_left_encoder(void);

uint16_t read_right_encoder(void);

#endif //SIMPLEFOC_STM32F431_ENCODER_H
