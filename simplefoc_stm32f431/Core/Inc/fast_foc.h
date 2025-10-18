#ifndef _fast_foc_h_
#define _fast_foc_h_


#include "tim.h"
#include "encoder.h"

#define  FOC_MOTOR_KV_NUM               (650.0f)        // 驱动的电机KV值  用于超前角的计算  大幅度减小 高转速时的发热情况

#define  FOC_TRACTION_ANGLE     (90.0f)         // 快速FOC模式下   正转的牵引角度，默认为90°

#define  FOC_PREACT_ANGLE       (70.0f)         // 快速FOC模式下   由于高转速时的牵引滞后导致异常发热电流  通过超前角的方式防止该情况 大幅度减小 高转速时的发热情况 默认70° （要求电机KV值准确）

#define USE_SIN_DATA    0       // 1 使用正弦波输出   0  使用马鞍波输出

#define     func_abs(x)             ((x) >= 0 ? (x): -(x))

#define     func_limit(x, y)        ((x) > (y) ? (y) : ((x) < -(y) ? -(y) : (x)))

#define     func_limit_ab(x, a, b)  ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

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

    uint8_t traction_angle;
    uint8_t preact_angle;

    int32_t encoder_now_data;
    int32_t encoder_last_data;

    int64_t rotate;
    int64_t rotate_last;
    int32_t speed_raw;
    uint32_t speed_calc_time;
    int32_t speed_actual_sum;
    int32_t speed_actual_result;
    int32_t speed_filtered;
    uint16_t ouput_duty[3];
} fast_foc_struct;


extern fast_foc_struct motor_left_foc_driver;

extern fast_foc_struct motor_right_foc_driver;


void mos_init(TIM_HandleTypeDef *_htim);

void foc_control_fast(fast_foc_struct *fast_foc_pointer, int32_t now_encoder_data, float output_duty_max);

void foc_control_fast_init(TIM_HandleTypeDef *htim, fast_foc_struct *fast_foc_pointer, int32_t encoder_max_data,
                           int32_t duty_max_data, uint32_t motor_pole_pairs, int32_t zero_location,
                           int32_t rotation_direction, int16_t traction_angle, int16_t preact_angle);


#endif
