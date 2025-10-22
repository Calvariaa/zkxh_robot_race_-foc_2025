#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "stdint.h"
#include "stdbool.h"

typedef enum {
  STOP = 0,
  START_ZEBRA,
  START,
  STOP_ZEBRA
} control_state_t;

typedef struct {
  uint8_t step;
  bool state;
  bool key_short;
  uint16_t time;
  bool key_long;
} key_struct_t;

extern int16_t accel[3], gyro[3];

extern control_state_t control_state;
extern int16_t turn_error;
extern key_struct_t key;

extern int32_t count;
extern int32_t last_count;
extern uint8_t direction;

void key_scan(void);

#endif
