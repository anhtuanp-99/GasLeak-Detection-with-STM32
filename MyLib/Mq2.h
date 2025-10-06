// Mq2.h
#ifndef MQ2_H
#define MQ2_H
#include "stdint.h"
#include "stm32f1xx_hal.h"

#define Vref            3.3    // Điện áp tham chiếu ADC
#define ADC_MAX_VALUE   4095   // Giá trị tối đa ADC 12-bit
#define RL              10     // Điện trở nối tiếp với RS
#define RATIO_CLEAN_AIR 9.83   // RS/R0 trong không khí sạch

typedef struct {
    float a;              // Điểm chân
    float b;              // Độ dốc
    float R0;             // Điện trở trong không khí sạch
    float RS_air;         // RS trong không khí sạch
    float RS_gas;         // RS trong khí gas
    float ppm;            // Nồng độ khí (ppm)
    float ratino;         // Tỷ lệ RS_gas/R0
    float Sensor_volt;    // Điện áp đọc từ ADC
    float percent_result; // Kết quả phần trăm
} Tyde_Def_MQ2;

void Mq2_init(Tyde_Def_MQ2 *_mq2, ADC_HandleTypeDef *_hadc, float a, float b);
void Mq2_calibrate(Tyde_Def_MQ2 *_mq2);
void Mq2_handle(Tyde_Def_MQ2 *_mq2);
void set_Sensor_volt(Tyde_Def_MQ2 *_mq2, uint16_t _adcValue);
void set_RS_air(Tyde_Def_MQ2 *_mq2);
void set_RO(Tyde_Def_MQ2 *_mq2);
void set_RS_gas(Tyde_Def_MQ2 *_mq2);
void set_ratino(Tyde_Def_MQ2 *_mq2);
void set_ppm(Tyde_Def_MQ2 *_mq2);
void set_percent_result(Tyde_Def_MQ2 *_mq2);

#endif