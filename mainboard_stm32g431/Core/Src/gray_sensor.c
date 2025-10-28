#include "gray_sensor.h"

#include "control.h"
#include "gpio.h"

bool gray_forward_value[MAX_FRONT_LED] = {0};
bool gray_backward_value[MAX_BACK_LED] = {0};

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

void outline_check(void) {
  bool outline = true;
  for (uint8_t i = 0; i < MAX_FRONT_LED; i++) {
    if (gray_forward_value[i]) {
      outline = false;
      return;
    }
  }
  control_state = STOP;
}

int8_t calc_left_right_error(void) {
  uint8_t left = 0;
  uint8_t right = 18;
  for (uint8_t i = 0; i < MAX_FRONT_LED; i++) {
    if (gray_forward_value[i]) {
      left = i;
    }
  }
  for (uint8_t i = MAX_FRONT_LED; i > 0; i--) {
    if (gray_forward_value[i - 1]) {
      right = i;
    }
  }
  return left + right - MAX_FRONT_LED;
}

int8_t calc_left_right_error_enhanced(void) {
  uint8_t left_start = 0;
  uint8_t left_end = 18;
  uint8_t right_start = 18;
  uint8_t right_end = 0;

  // 寻找左边第一个白色点
  for (uint8_t i = 0; i < MAX_FRONT_LED; i++) {
    if (gray_forward_value[i]) {
      left_start = i;
      break;
    }
  }

  // 寻找右边第一个白色点
  for (uint8_t i = MAX_FRONT_LED; i > 0; i--) {
    if (gray_forward_value[i - 1]) {
      right_start = i - 1;
      break;
    }
  }

  // 如果左右都没有检测到白线，返回0（保持直行或特殊处理）
  if (left_start == 0 && !gray_forward_value[0] &&
      right_start == 18 && !gray_forward_value[17]) {
    return 0;
      }

  // 从左边第一个白色点继续向右寻找白色点结束的位置
  for (uint8_t i = left_start; i < MAX_FRONT_LED; i++) {
    if (!gray_forward_value[i]) {
      left_end = i;
      break;
    }
    // // 如果一直到最右边都是白色，则结束位置为最右边
    // if (i == MAX_FRONT_LED - 1) {
    //   left_end = i;
    // }
  }

  // 从右边第一个白色点继续向左寻找白色点结束的位置
  for (uint8_t i = right_start; i > 0; i--) {
    if (!gray_forward_value[i]) {
      right_end = i;
      break;
    }
    // // 如果一直到最左边都是白色，则结束位置为最左边
    // if (i == 0) {
    //   right_end = i;
    // }
  }

  // 检查左边第一个白色点到右边第一个白色点的范围内是否有白色点
  bool has_white_points = false;
  for (uint8_t i = left_end; i <= right_end; i++) {
    if (gray_forward_value[i]) {
      has_white_points = true;
      break;
    }
  }

  if (has_white_points || left_end > right_end) {
    // 正常循迹：使用左右第一个白色点计算中值
    return left_start + right_start - MAX_FRONT_LED + 1;
  }

    // 特殊处理：使用最右边第一个白色点和白色点结束位置计算中值
    return right_end + right_start - MAX_FRONT_LED + 2;
}
