//
// Created by Calvaria on 2025/9/20.
//

#ifndef SIMPLEFOC_STM32F431_FOC_H
#define SIMPLEFOC_STM32F431_FOC_H

#include "tim.h"
#include "encoder.h"

#define ANGLE_TO_RAD(x)     ( (x) * M_PI / 180.0 )                           // 角度转换为弧度
#define RAD_TO_ANGLE(x)     ( (x) * 180.0 / M_PI )                           // 弧度转换为角度

typedef struct {
        float a;
        float b;
        float c;
}motor_t;

typedef struct {
        float i_d;
        float i_q;
}turn_vector_t;

typedef struct {
        float alpha;
        float beta;
}static_vector_t;

typedef struct {
        TIM_HandleTypeDef *htim;

        motor_t current;
        turn_vector_t turn_vector;
        encoder_t encoder;
        static_vector_t i_park_vector;
        uint8_t sector;
        motor_t sector_voltage;
        static_vector_t svpwm_vector;
        motor_t svpwm_voltage;

        float set_angle;
        uint32_t expect_rotations;
}foc_t;

extern foc_t foc_L, foc_R;

void foc_init(foc_t *_foc, TIM_HandleTypeDef *_htim);

void foc_control(foc_t *_foc, int32_t now_encoder_data);

void mos_init(TIM_HandleTypeDef *_htim);

#endif //SIMPLEFOC_STM32F431_FOC_H
