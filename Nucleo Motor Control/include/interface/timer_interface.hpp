#ifndef __TIMER_INTERFACE
#define __TIMER_INTERFACE

#include "stm32g4xx_hal.h"
#include "types.hpp"
#include <functional>

namespace STM_TIMER
{
    bool init();
    void registerCallback(int timer, std::function<void()> callback);
    void setPeriod(int timer, float period);
    void setFrequency(int timer, float frequency);
}

#endif // __TIMER_INTERFACE