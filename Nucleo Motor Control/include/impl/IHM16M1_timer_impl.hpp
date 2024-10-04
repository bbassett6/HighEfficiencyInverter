#ifndef __IHM16M1_TIMER_IMPL
#define __IHM16M1_TIMER_IMPL

#include "interface/timer_interface.hpp"
#include "types.hpp"

#if PLATFORM_P_NUCLEO_IHM03

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

namespace IHM16M1TIMER
{
    // TIM1
    // Used by SVM timing
    std::function<void()> _tim1Callback;

    // TIM2
    // Used by ADC auto sampler
    std::function<void()> _tim2Callback;
}

#endif // PLATFORM_P_NUCLEO_IHM03
#endif // __IHM16M1_ADC_IMPL