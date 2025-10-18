#ifndef __GRAY_SENSOR_H__
#define __GRAY_SENSOR_H__

#include "stdbool.h"

extern bool gray_forward_value[18];
extern bool gray_backward_value[6];

void read_gray_sensor(void);

#endif