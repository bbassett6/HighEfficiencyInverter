#ifndef __TYPES
#define __TYPES

#include "common.hpp"

enum Phase
{
    A = 0,
    B = 1,
    C = 2,
    NumPhases = 3
};

template <typename T>
union Vec3
{
    struct u_s
    {
        T A;
        T B;
        T C;
    } u;
    T a[3];
};

template <typename T>
union Vec2
{
    struct u_s
    {
        T A;
        T B;
    } u;
    T a[2];
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