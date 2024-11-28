#include "interface/timer_interface.hpp"
#include "types.hpp"

#if PLATFORM_HEI

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

namespace STM_TIMER
{
    // TIM2
    // Used by speed observer (hrTick)
    std::function<void()> _tim2Callback;

    uint16_t tim2period = 0;
    volatile unsigned long accumulatedTimer = 0;
    unsigned long tim2base = 0;

    // TIM3
    // Used by ADC auto sampler
    std::function<void()> _tim3Callback;

    bool init()
        {
        // Timer 2 init
        TIM_ClockConfigTypeDef sClockSourceConfig = {0};
        TIM_MasterConfigTypeDef sMasterConfig = {0};
        __HAL_RCC_TIM2_CLK_ENABLE();
        htim2.Instance = TIM2;
        htim2.Init.Prescaler = 0;
        htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim2.Init.Period = 20000;
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
        __HAL_RCC_TIM3_CLK_ENABLE();
        htim3.Instance = TIM3;
        htim3.Init.Prescaler = 0;
        htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim3.Init.Period = 60000;
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
        
        __NVIC_EnableIRQ(TIM2_IRQn);
        __NVIC_EnableIRQ(TIM3_IRQn);

        if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
            return false;
        if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
            return false;

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
        unsigned long timerCycles = nanos * PlatformParameters::cpuMHz / 1000.0f;
        unsigned int prescaler = 0;

        if (timer == 2)
        {
            while (timerCycles > UINT16_MAX)
            {
                timerCycles /= 2;
                prescaler = (prescaler + 1) * 2 - 1;
            }

            tim2period = timerCycles;

            __HAL_TIM_SET_AUTORELOAD(&htim2, (int)timerCycles);
            __HAL_TIM_SET_PRESCALER(&htim2, 0);
        }

        if (timer == 3)
        {
            while (timerCycles > UINT16_MAX)
            {
                timerCycles /= 2;
                prescaler = (prescaler + 1) * 2 - 1;
            }

            __HAL_TIM_SET_AUTORELOAD(&htim3, (int)timerCycles);
            __HAL_TIM_SET_PRESCALER(&htim3, prescaler);
        }
    }

    void setFrequency(int timer, float frequency)
    {
        if (timer == 2 || timer == 3)
        {
            setPeriod(timer, 1000000000.0f / frequency);
        }
    }

    void hrTickReset()
    {
        tim2base = accumulatedTimer + __HAL_TIM_GET_COUNTER(&htim2);
    }

    unsigned long hrTickGet()
    {
        return accumulatedTimer + __HAL_TIM_GET_COUNTER(&htim2) - tim2base;
    }
}

extern "C" void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

extern "C" void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim3);
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == &htim2)
    {
        STM_TIMER::accumulatedTimer += STM_TIMER::tim2period;
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