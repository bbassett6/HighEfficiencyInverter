#ifndef __TYPES
#define __TYPES

#include "common.hpp"

enum Implementation
{
    IHM161M1 =          0,
    HEI =               1,
    NumImplementations
};

template <typename T>
struct Triple
{
    T A;
    T B;
    T C;
};

template <typename T>
struct Vec2
{
    T A;
    T B;
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