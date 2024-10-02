#ifndef __IHM16M1_TIMER_IMPL
#define __IHM16M1_TIMER_IMPL

#include "stm32g4xx_hal.h"
#include "parameters.h"
#include "peripherals.h"
#include "types.hpp"
#include "interface/timer_interface.hpp"

#if PLATFORM_P_NUCLEO_IHM03

namespace IHM16M1TIMER
{
    // TIM1
    std::function<void()> _tim1Callback;
}

#endif // PLATFORM_P_NUCLEO_IHM03
#endif // __IHM16M1_ADC_IMPL