#include "impl/HEI_ADC_impl.hpp"

#if PLATFORM_HEI

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

namespace STM_ADC
{
    bool _initialized =                     false;
    HEIADC::PinNames _currentPin =          static_cast<HEIADC::PinNames>(0);
    float _effectiveRate =                  10000;
    int _oversampleFactor =                 2;
    unsigned long _timerPeriodNanos =       0; // the period of the timer used to automatically advance the ADC state machine
    std::function<void(int, int)> _userCallback; // _userCallback(HEIADC::PinNames channel, int result)

    // Configure ADC parameters here
    ADC_ChannelConfTypeDef _channelConfig = {
        .Channel        = 0, // nonsense value, must be set before _channelConfig can be used
        .Rank 			= ADC_REGULAR_RANK_1,
        .SamplingTime 	= ADC_SAMPLETIME_2CYCLES_5,
        .SingleDiff 	= ADC_SINGLE_ENDED,
        .OffsetNumber 	= ADC_OFFSET_NONE,
        .Offset 		= 0,
    };

    bool init()
    {
        // Set GPIO modes
        for (PinDef pin : HEIADC::PinDefs)
        {
            HAL_GPIO_Init(pin.port, &pin.init);
        }

        // Initialize ADC1
        if (__HAL_RCC_ADC12_IS_CLK_DISABLED())
            __HAL_RCC_ADC12_CLK_ENABLE();
        
        ADC_MultiModeTypeDef multimode = {0};

        hadc1.Instance = ADC1;
        hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
        hadc1.Init.Resolution = ADC_RESOLUTION_12B;
        hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
        hadc1.Init.ContinuousConvMode = DISABLE;
        hadc1.Init.DiscontinuousConvMode = DISABLE;
        hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        hadc1.Init.NbrOfConversion = 1;
        hadc1.Init.DMAContinuousRequests = DISABLE;
        hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        hadc1.Init.LowPowerAutoWait = DISABLE;
        hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
        if (HAL_ADC_Init(&hadc1) != HAL_OK)
        {
            return false;
        }
        multimode.Mode = ADC_MODE_INDEPENDENT;
        if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
        {
            return false;
        }

        // Initialize ADC2
        hadc2.Instance = ADC2;
        hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
        hadc2.Init.Resolution = ADC_RESOLUTION_12B;
        hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
        hadc2.Init.ContinuousConvMode = DISABLE;
        hadc2.Init.DiscontinuousConvMode = DISABLE;
        hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        hadc2.Init.NbrOfConversion = 1;
        hadc2.Init.DMAContinuousRequests = DISABLE;
        hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        hadc2.Init.LowPowerAutoWait = DISABLE;
        hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
        if (HAL_ADC_Init(&hadc2) != HAL_OK)
        {
            return false;
        }

        // Configure ADCs
        ADC_ChannelConfTypeDef sConfig = {0};
        for (AdcDef adcDef : HEIADC::AdcDefs)
        {
            sConfig.Channel 		= adcDef.channel;

            if (HAL_ADC_ConfigChannel(adcDef.adc == 1 ? &hadc1 : &hadc2, &sConfig) != HAL_OK)
            {
                return false;
            }
        }

        // Enable interrupts
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

        _initialized = true;
        
        // Initialization completed successfully
        return true;
    }

    void setRate(float rate)
    {
        _effectiveRate = rate;
        _timerPeriodNanos = 1e9 / (_effectiveRate * HEIADC::PinNames::NumPins);

        STM_TIMER::setPeriod(3, _timerPeriodNanos);
    }

    void setCallback(std::function<void(int, int)> callback)
    {
        _userCallback = callback;
    }

    // Handles the ADC state machine
    void _adcConversionCallback()
    {
        if (_initialized)
        {
            // call user's callback if it exists
            if (_userCallback)
            {
                _userCallback(static_cast<int>(_currentPin), HAL_ADC_GetValue([]() -> ADC_HandleTypeDef * {
                    if (HEIADC::AdcDefs[_currentPin].adc == 1)
                        return &hadc1;
                    return &hadc2;
                }()));
            }
            // advance the pin being sampled 
            _currentPin = static_cast<HEIADC::PinNames>((_currentPin + 1) % HEIADC::PinNames::NumPins);

            // select the correct channel on the correct ADC
            const AdcDef* adcDef = &HEIADC::AdcDefs[_currentPin];
            _channelConfig.Channel = adcDef->channel;
            HAL_ADC_ConfigChannel(adcDef->adc == 1 ? &hadc1 : &hadc2, &_channelConfig);

            // start interrupting conversion
            HAL_ADC_Start_IT(adcDef->adc == 1 ? &hadc1 : &hadc2);
        }
    }
}

// Configure interrupt handlers for ADC1 and ADC2
extern "C" void ADC1_IRQHandler()
{
    HAL_ADC_IRQHandler(&hadc1);
}

extern "C" void ADC2_IRQHandler()
{
    HAL_ADC_IRQHandler(&hadc2);
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    STM_ADC::_adcConversionCallback();
}

#endif // PLATFORM_HEI