#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct
{
    uint8_t step;
    bool state;
    bool key_short;
    uint16_t time;
    bool key_long;
}key_struct_t;

extern key_struct_t key;

extern int32_t count;
extern int32_t last_count;
extern uint8_t direction;

void key_scan(void);

#endif