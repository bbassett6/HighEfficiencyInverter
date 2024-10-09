#include "impl/HEI_timer_impl.hpp"

#if PLATFORM_HEI

namespace STM_TIMER
{
    // TIM2
    // Used by SVM timing
    std::function<void()> _tim2Callback;

    // TIM3
    // Used by ADC auto sampler
    std::function<void()> _tim3Callback;

    bool init()
    {
        // Timer 2 init
        TIM_ClockConfigTypeDef sClockSourceConfig = {0};
        TIM_MasterConfigTypeDef sMasterConfig = {0};
        htim2.Instance = TIM2;
        htim2.Init.Prescaler = 0;
        htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim2.Init.Period = 4294967295;
        htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
        {
            return false;
        }
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
        {
            return false;
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
        {
            return false;
        }

        // Timer 3 init
        sClockSourceConfig = {0};
        sMasterConfig = {0};
        htim3.Instance = TIM3;
        htim3.Init.Prescaler = 0;
        htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim3.Init.Period = 65535;
        htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
        {
            return false;
        }
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
        {
            return false;
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
        {
            return false;
        }

        return true;
    }

    void registerCallback(int timer, std::function<void()> callback)
    {
        if (timer == 2)
        {
            _tim2Callback = callback;
        }
        if (timer == 3)
        {
            _tim3Callback = callback;
        }
    }

    void setPeriod(int timer, unsigned long nanos)
    {
        unsigned long timerCycles = nanos * 1000 * cpuMHz;
        int prescaler = 0;

        if (timer == 2)
        {
            while (timerCycles > (unsigned long)UINT32_MAX)
            {
                timerCycles /= 2;
                prescaler++;
            }

            __HAL_TIM_SET_AUTORELOAD(&htim2, (int)timerCycles);
            __HAL_TIM_SET_PRESCALER(&htim2, prescaler);
        }

        if (timer == 3)
        {
            while (timerCycles > UINT16_MAX)
            {
                timerCycles /= 2;
                prescaler++;
            }

            __HAL_TIM_SET_AUTORELOAD(&htim3, (int)timerCycles);
            __HAL_TIM_SET_PRESCALER(&htim3, prescaler);
        }
    }

    void setFrequency(int timer, float frequency)
    {
        if (timer == 2 || timer == 2)
        {
            setPeriod(timer, 1.0f / frequency);
        }
    }

}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == &htim2)
    {
        if (STM_TIMER::_tim2Callback)
            STM_TIMER::_tim2Callback();
    }

    if (htim == &htim3)
    {
        if (STM_TIMER::_tim3Callback)
            STM_TIMER::_tim3Callback();
    }
}

#endif // PLATFORM_HEI