#ifndef __TIMER_INTERFACE
#define __TIMER_INTERFACE

#include "common.hpp"
#include <functional>

namespace STM_TIMER
{
    bool init();
    void registerCallback(int timer, std::function<void()> callback);
    void setPeriod(int timer, unsigned long nanos);
    void setFrequency(int timer, float frequency);
}

#endif // __TIMER_INTERFACE