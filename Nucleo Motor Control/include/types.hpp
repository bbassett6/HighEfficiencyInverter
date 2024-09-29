#ifndef __TYPES
#define __TYPES

#include "stm32g4xx_hal.h"

enum Implementation
{
    IHM161M1 =          0,
    HEI =               1,
    NumImplementations
};

template <typename T>
struct Triple
{
    T U;
    T V;
    T W;
};

struct PinDef
{
    GPIO_TypeDef* port;
    GPIO_InitTypeDef init;
};

struct AdcDef
{
    uint32_t adc;
    uint32_t channel;
};

struct ADC
{
    float CH0;
    float CH1;
    float CH2;
    float CH3;
};

#endif // __TYPES