#include "gray_sensor.h"

#include "gpio.h"

bool gray_forward_value[18];
bool gray_backward_value[6];

void read_gray_sensor(void) {
    gray_forward_value[0] = HAL_GPIO_ReadPin(LED_F0_GPIO_Port, LED_F0_Pin);
    gray_forward_value[1] = HAL_GPIO_ReadPin(LED_F1_GPIO_Port, LED_F1_Pin);
    gray_forward_value[2] = HAL_GPIO_ReadPin(LED_F1_GPIO_Port, LED_F2_Pin);
    gray_forward_value[3] = HAL_GPIO_ReadPin(LED_F3_GPIO_Port, LED_F3_Pin);
    gray_forward_value[4] = HAL_GPIO_ReadPin(LED_F4_GPIO_Port, LED_F4_Pin);
    gray_forward_value[5] = HAL_GPIO_ReadPin(LED_F5_GPIO_Port, LED_F5_Pin);
    gray_forward_value[6] = HAL_GPIO_ReadPin(LED_F6_GPIO_Port, LED_F6_Pin);
    gray_forward_value[7] = HAL_GPIO_ReadPin(LED_F7_GPIO_Port, LED_F7_Pin);
    gray_forward_value[8] = HAL_GPIO_ReadPin(LED_F8_GPIO_Port, LED_F8_Pin);
    gray_forward_value[9] = HAL_GPIO_ReadPin(LED_F9_GPIO_Port, LED_F9_Pin);
    gray_forward_value[10] = HAL_GPIO_ReadPin(LED_F10_GPIO_Port, LED_F10_Pin);
    gray_forward_value[11] = HAL_GPIO_ReadPin(LED_F11_GPIO_Port, LED_F11_Pin);
    gray_forward_value[12] = HAL_GPIO_ReadPin(LED_F12_GPIO_Port, LED_F12_Pin);
    gray_forward_value[13] = HAL_GPIO_ReadPin(LED_F13_GPIO_Port, LED_F13_Pin);
    gray_forward_value[14] = HAL_GPIO_ReadPin(LED_F14_GPIO_Port, LED_F14_Pin);
    gray_forward_value[15] = HAL_GPIO_ReadPin(LED_F15_GPIO_Port, LED_F15_Pin);
    gray_forward_value[16] = HAL_GPIO_ReadPin(LED_F16_GPIO_Port, LED_F16_Pin);
    gray_forward_value[17] = HAL_GPIO_ReadPin(LED_F17_GPIO_Port, LED_F17_Pin);

    gray_backward_value[0] = HAL_GPIO_ReadPin(LED_B0_GPIO_Port, LED_B0_Pin);
    gray_backward_value[1] = HAL_GPIO_ReadPin(LED_B1_GPIO_Port, LED_B1_Pin);
    gray_backward_value[2] = HAL_GPIO_ReadPin(LED_B2_GPIO_Port, LED_B2_Pin);
    gray_backward_value[3] = HAL_GPIO_ReadPin(LED_B3_GPIO_Port, LED_B3_Pin);
    gray_backward_value[4] = HAL_GPIO_ReadPin(LED_B4_GPIO_Port, LED_B4_Pin);
    gray_backward_value[5] = HAL_GPIO_ReadPin(LED_B5_GPIO_Port, LED_B5_Pin);
}