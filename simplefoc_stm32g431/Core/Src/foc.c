//
// Created by Calvaria on 2025/9/20.
//

#define BUS_VOLTAGE 12

#include "../Inc/foc.h"
#include <math.h>
#include "arm_math.h"

void mos_init(TIM_HandleTypeDef *_htim) {
    HAL_TIM_PWM_Start(_htim,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(_htim,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(_htim,TIM_CHANNEL_3);

    HAL_TIMEx_PWMN_Start(_htim,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(_htim,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(_htim,TIM_CHANNEL_3);
}

void mos_all_set(const TIM_HandleTypeDef *_htim, const uint32_t _cmp_u, const uint32_t _cmp_v, const uint32_t _cmp_w) {
    __HAL_TIM_SET_COMPARE(_htim, TIM_CHANNEL_1, _cmp_u);
    __HAL_TIM_SET_COMPARE(_htim, TIM_CHANNEL_2, _cmp_v);
    __HAL_TIM_SET_COMPARE(_htim, TIM_CHANNEL_3, _cmp_w);
}

void mos_all_close(const TIM_HandleTypeDef *_htim) {
    mos_all_set(_htim, 0, 0, 0);
}

static_vector_t i_park(const turn_vector_t _turn_vector, const float _phi) {
    static_vector_t static_vector;
    static_vector.alpha = _turn_vector.i_d * arm_cos_f32(_phi) - _turn_vector.i_q * arm_sin_f32(_phi);
    static_vector.beta = _turn_vector.i_d * arm_sin_f32(_phi) + _turn_vector.i_q * arm_cos_f32(_phi);
    return static_vector;
}

motor_t i_clark(const static_vector_t _static_vector) {
    motor_t motor;
    motor.a = _static_vector.alpha;
    motor.b = _static_vector.beta * M_SQRT3 / 2 - _static_vector.alpha / 2;
    motor.c = -_static_vector.beta * M_SQRT3 / 2 - _static_vector.alpha / 2;
    return motor;
}

uint8_t sector_calc(const motor_t _motor) {
    uint8_t N = 0;

    if (_motor.a > 0)
        N = N + 1;
    if (_motor.b > 0)
        N = N + 2;
    if (_motor.c > 0)
        N = N + 4;

    return N;
}

static_vector_t sector_time(const uint8_t _sec, const motor_t _motor) {
    static_vector_t vector;

    float temp = M_SQRT3 * COUNT_PERIOD / BUS_VOLTAGE; // 为了等幅值变换，已乘以2/3

    switch (_sec) {
        case 3: // 扇区1
            vector.alpha = temp * _motor.b;
            vector.beta = temp * _motor.a;
            break;
        case 1: // 扇区2
            vector.alpha = -temp * _motor.b;
            vector.beta = -temp * _motor.c;
            break;
        case 5: // 扇区3
            vector.alpha = temp * _motor.a;
            vector.beta = temp * _motor.c;
            break;
        case 4: // 扇区4
            vector.alpha = -temp * _motor.a;
            vector.beta = -temp * _motor.b;
            break;
        case 6: // 扇区5
            vector.alpha = temp * _motor.c;
            vector.beta = temp * _motor.b;
            break;
        case 2: // 扇区6
            vector.alpha = -temp * _motor.c;
            vector.beta = -temp * _motor.a;
            break;
        default:
            vector.alpha = 0;
            vector.beta = 0;
            break;
    }

    return vector;
}

motor_t period_calc(static_vector_t _vector, const uint8_t _sector, const uint32_t _period) {
    motor_t period;
    const float T_temp = _vector.alpha + _vector.beta;

    if (T_temp > _period) {
        _vector.alpha = _vector.alpha / T_temp * (float) _period;
        _vector.beta = _vector.beta / T_temp * (float) _period;
    }
    const float value1 = (((float) _period - _vector.alpha - _vector.beta) / 2.0f);
    const float value2 = (value1 + _vector.alpha);
    const float value3 = (value2 + _vector.beta);
    switch (_sector) {
        case 3:
            period.a = value1;
            period.b = value2;
            period.c = value3;
            break;
        case 1:
            period.a = value2;
            period.b = value1;
            period.c = value3;
            break;
        case 5:
            period.a = value3;
            period.b = value1;
            period.c = value2;
            break;
        case 4:
            period.a = value3;
            period.b = value2;
            period.c = value1;
            break;
        case 6:
            period.a = value2;
            period.b = value3;
            period.c = value1;
            break;
        case 2:
            period.a = value1;
            period.b = value3;
            period.c = value2;
            break;
        default:
            period.a = (uint32_t) (COUNT_PERIOD / 2);
            period.b = (uint32_t) (COUNT_PERIOD / 2);
            period.c = (uint32_t) (COUNT_PERIOD / 2);
            break;
    }

    return period;
}

foc_t foc_L, foc_R;

void foc_init(foc_t *_foc, TIM_HandleTypeDef *_htim) {
    _foc->htim = _htim;
    _foc->set_angle = 0;
    _foc->expect_rotations = 0;
}

void foc_control(foc_t *_foc, int32_t now_encoder_data) {
    // _foc->set_angle += ANGLE_TO_RAD(0.4);
    _foc->set_angle += ANGLE_TO_RAD(0.08);
    if (_foc->set_angle >= M_PI * 2) {
        _foc->expect_rotations++;
        _foc->set_angle -= M_PI * 2;
    }
    if (_foc->set_angle < -M_PI * 2) {
        _foc->expect_rotations--;
        _foc->set_angle += M_PI * 2;
    }

    _foc->turn_vector.i_d = 0;
    _foc->turn_vector.i_q = 1;

    _foc->i_park_vector = i_park(_foc->turn_vector, ((float)(now_encoder_data) * M_PI * 2 * 10 / 16384));
    _foc->sector_voltage = i_clark(_foc->i_park_vector);
    _foc->sector = sector_calc(_foc->sector_voltage);
    _foc->svpwm_vector = sector_time(_foc->sector, _foc->sector_voltage);
    _foc->svpwm_voltage = period_calc(_foc->svpwm_vector, _foc->sector, COUNT_PERIOD / 2);

    mos_all_set(_foc->htim, _foc->svpwm_voltage.a, _foc->svpwm_voltage.b, _foc->svpwm_voltage.c);
}
