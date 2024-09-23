#ifndef __TYPES
#define __TYPES

#include "stm32g4xx_hal.h"

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

#endif // __TYPES