#include "impl/IHM16M1_timer_impl.hpp"

#if PLATFORM_P_NUCLEO_IHM03

namespace STM_TIMER
{
    bool init()
    {
        // initialize timer 1
        TIM_ClockConfigTypeDef sClockSourceConfig = {0};
        TIM_MasterConfigTypeDef sMasterConfig = {0};

        htim1.Instance = TIM1;
        htim1.Init.Prescaler = 0;										// No prescaler gives us max resolution
        htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim1.Init.Period = 65535;
        htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim1.Init.RepetitionCounter = 0;
        htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        
        if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
        {
            return false;
        }
        
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        
        if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
        {
            return false;
        }
        
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        
        if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
        {
            return false;
        }

        // initialized successfully
        return true;
    }

    void registerCallback(int timer, std::function<void()> callback)
    {
        if (timer == 1)
        {
            IHM16M1TIMER::_tim1Callback = callback;
        }
    }

    void setPeriod(int timer, float period)
    {
        if (timer == 1)
        {
            int timerCycles = period * cpuMHz * 1000000;
            int prescaler = 0;
            
            if (timerCycles > UINT16_MAX) {
                // Set prescaler to the maximum value that keeps arr within bounds
                prescaler = timerCycles / (UINT16_MAX + 1);
                timerCycles = timerCycles / (prescaler + 1);
            }

            __HAL_TIM_SET_AUTORELOAD(&htim1, timerCycles);
            __HAL_TIM_SET_PRESCALER(&htim1, prescaler);
        }
    }

    void setFrequency(int timer, float frequency)
    {
        if (timer == 1)
        {
            setPeriod(1, 1.0f / frequency);
        }
    }

}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == &htim1)
    {
        if (IHM16M1TIMER::_tim1Callback)
        {
            IHM16M1TIMER::_tim1Callback();
        }
    }
}

#endif // PLATFORM_P_NUCLEO_IHM03