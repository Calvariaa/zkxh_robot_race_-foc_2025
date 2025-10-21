#include "control.h"
#include "i2c.h"
#include "tim.h"
#include "icm42688.h"
#include "stdio.h"


key_struct_t key;


void key_scan(void) {
    key.state = HAL_GPIO_ReadPin(PRESS_GPIO_Port,PRESS_Pin);

    switch (key.step) {
        case 0:
            if (key.state == 0) {
                key.time = 0;
                key.step = 1;
            }
            break;
        case 1:
            if (key.state == 0) {
                key.step = 2;
            } else {
                key.step = 0;
            }
            break;
        case 2:
            if (key.state == 0) {
                key.time++;
                if (key.time > 50) {
                    key.key_long = 1;
                }
            } else {
                key.step = 0;
                if (key.time < 50) {
                    key.key_short = 1;
                }
            }
            break;
        default:
            break;
    }
}
