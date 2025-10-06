// Mq2.c
#include "Mq2.h"
#include "math.h"

ADC_HandleTypeDef *hadc;

__WEAK void Mq2_callback(float value) {
    // Hàm callback mặc định
}

void Mq2_init(Tyde_Def_MQ2 *_mq2, ADC_HandleTypeDef *_hadc, float a, float b) {
    hadc = _hadc;
    _mq2->a = a;
    _mq2->b = b;
}

void Mq2_calibrate(Tyde_Def_MQ2 *_mq2) {
    HAL_ADC_Start(hadc);
    if (HAL_ADC_PollForConversion(hadc, 1000) == HAL_OK) {
        uint16_t value = HAL_ADC_GetValue(hadc);
        set_Sensor_volt(_mq2, value);
        set_RS_air(_mq2);
        set_RO(_mq2);
    }
}

void Mq2_handle(Tyde_Def_MQ2 *_mq2) {
    HAL_ADC_Start(hadc);
    if (HAL_ADC_PollForConversion(hadc, 1000) == HAL_OK) {
        uint16_t value = HAL_ADC_GetValue(hadc);
        set_Sensor_volt(_mq2, value);
        set_RS_gas(_mq2);
        set_ratino(_mq2);
        set_ppm(_mq2);
        set_percent_result(_mq2);
    }
    HAL_ADC_Stop(hadc);
}

void set_Sensor_volt(Tyde_Def_MQ2 *_mq2, uint16_t _adcValue) {
    uint32_t sum = 0;
    const uint8_t num_samples = 5;
    for (uint8_t i = 0; i < num_samples; i++) {
        HAL_ADC_Start(hadc);
        if (HAL_ADC_PollForConversion(hadc, 1000) == HAL_OK) {
            sum += HAL_ADC_GetValue(hadc);
        }
    }
    float avg_adc = (float)sum / num_samples;
    if (avg_adc == 0) {
        _mq2->Sensor_volt = 0.0001f; // Tránh chia cho 0
        return;
    }
    _mq2->Sensor_volt = (Vref * avg_adc / ADC_MAX_VALUE);
}

void set_RS_air(Tyde_Def_MQ2 *_mq2) {
    _mq2->RS_air = ((3.3 * RL) / _mq2->Sensor_volt) - RL;
}

void set_RS_gas(Tyde_Def_MQ2 *_mq2) {
    _mq2->RS_gas = ((3.3 * RL) / _mq2->Sensor_volt) - RL;
}

void set_RO(Tyde_Def_MQ2 *_mq2) {
    _mq2->R0 = _mq2->RS_air / RATIO_CLEAN_AIR;
}

void set_ratino(Tyde_Def_MQ2 *_mq2) {
    _mq2->ratino = _mq2->RS_gas / _mq2->R0;
}

void set_ppm(Tyde_Def_MQ2 *_mq2) {
    if (_mq2->ratino <= 0) {
        _mq2->ppm = 0;
        return;
    }
    _mq2->ppm = pow(10, ((log10(_mq2->ratino) - _mq2->a) / _mq2->b));
}

void set_percent_result(Tyde_Def_MQ2 *_mq2) {
    _mq2->percent_result = _mq2->ppm / 10000;
    Mq2_callback(_mq2->percent_result);
}