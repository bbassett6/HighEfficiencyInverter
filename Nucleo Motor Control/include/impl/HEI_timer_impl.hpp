#ifndef __HEI_TIMER_IMPL
#define __HEI_TIMER_IMPL

#include "interface/timer_interface.hpp"
#include "types.hpp"

#if PLATFORM_HEI

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

namespace HEITIMER
{
    // TIM2
    // Used by SVM timing
    std::function<void()> _tim2Callback;

    // TIM3
    // Used by ADC auto sampler
    std::function<void()> _tim3Callback;
}

#endif // PLATFORM_HEI
#endif // __HEI_ADC_IMPL