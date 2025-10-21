#ifndef __GRAY_SENSOR_H__
#define __GRAY_SENSOR_H__

#include "stdint.h"
#include "stdbool.h"

#define MAX_FRONT_LED 18
#define MAX_BACK_LED 6

extern bool gray_forward_value[MAX_FRONT_LED];
extern bool gray_backward_value[MAX_BACK_LED];

void read_gray_sensor(void);
int8_t calc_left_right_error(void);

#endif