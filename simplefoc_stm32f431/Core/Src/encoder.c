//
// Created by Calvaria on 2025/9/20.
//

#include "../Inc/encoder.h"
#include <math.h>

// 归一化弧度
double normalize_rad(const float _theta) {
  const double a = fmod(_theta, M_PI * 2);
  return a >= 0 ? a : (a + M_PI * 2);
}

uint16_t read_left_encoder(void) {
  uint16_t u16Data;
  HAL_SPI_Receive( &hspi1,(uint8_t *)(&u16Data), sizeof(u16Data) / sizeof(uint16_t), HAL_MAX_DELAY);

  return (u16Data & 0x7fff);
}

uint16_t read_right_encoder(void) {
  uint16_t u16Data;
  HAL_SPI_Receive( &hspi3,(uint8_t *)(&u16Data), sizeof(u16Data) / sizeof(uint16_t), HAL_MAX_DELAY);

  return (u16Data & 0x7fff);
}
