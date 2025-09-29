#ifndef _fast_foc_h_
#define _fast_foc_h_


#include "tim.h"
#include "encoder.h"

#define USE_SIN_DATA    0       // 1 使用正弦波输出   0  使用马鞍波输出

typedef struct {
    float encoder_proportion;
    int32_t encoder_pole_pairs;
    float duty_proportion;
} fast_foc_calculate_struct;


typedef struct {
    TIM_HandleTypeDef *htim;

    fast_foc_calculate_struct calculate_value;

    int32_t encoder_max_data;
    int32_t duty_max_data;
    int32_t motor_pole_pairs;
    int32_t motor_zero_location;
    int32_t motor_rotation_direction;

    uint16_t ouput_duty[3];
} fast_foc_struct;


extern fast_foc_struct motor_left_foc_driver;

extern fast_foc_struct motor_right_foc_driver;


void mos_init(TIM_HandleTypeDef *_htim);

void foc_control_fast(fast_foc_struct *fast_foc_pointer, int32_t now_encoder_data, float output_duty_max,
                      int32_t traction_angle);

void foc_control_fast_init(TIM_HandleTypeDef *htim, fast_foc_struct *fast_foc_pointer, int32_t encoder_max_data,
                           int32_t duty_max_data, uint32_t motor_pole_pairs, int32_t zero_location,
                           int32_t rotation_direction);


#endif
