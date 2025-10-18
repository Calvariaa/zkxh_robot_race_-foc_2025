#include "control.h"
#include "i2c.h"
#include "tim.h"
#include "icm42688.h"
#include "stdio.h"
#include "usart.h"


key_t key;

int32_t count = 0;
int32_t last_count = 0;
uint8_t direction = 0;
int16_t accel[3], gyro[3];
int16_t send_data[4];
int16_t actual_speed[2];

uint16_t pit_time_1ms = 0, pit_time_2ms = 0, pit_time_5ms = 0, pit_time_10ms = 0;
uint64_t total_time_10ms = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // 1ms tim interrupt
    if (htim->Instance == TIM6) {
        pit_time_1ms++;
        pit_time_2ms++;
        pit_time_5ms++;
        pit_time_10ms++;

        if (pit_time_1ms == 1) {
            // 读取陀螺仪数据
            if (ICM42688_ReadGyro(&hi2c2, gyro) == HAL_OK) {
                printf("GX=%10d, GY=%10d, GZ=%10d\r\n", gyro[0], gyro[1], gyro[2]);
            }

            direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
            count = (int32_t) __HAL_TIM_GET_COUNTER(&htim2) >> 2;
            pit_time_1ms = 0;
        }
        if (pit_time_2ms == 2) {
            pit_time_2ms = 0;
        }
        if (pit_time_5ms == 5) {
            pit_time_5ms = 0;
        }
        if (pit_time_10ms == 10) {
            pit_time_10ms = 0;

            key_scan();
            total_time_10ms++;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        HAL_UART_Receive_IT(&huart1, (uint8_t *) actual_speed, sizeof(actual_speed) / sizeof(int16_t));
    }
}

void set_speed(void) {
    HAL_UART_Transmit_IT(&huart1, (uint8_t *)send_data, sizeof(send_data)/sizeof(int16_t));
}

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
