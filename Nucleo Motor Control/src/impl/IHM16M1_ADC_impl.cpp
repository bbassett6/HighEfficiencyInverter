#include "impl/IHM16M1_ADC_impl.hpp"

#if PLATFORM_P_NUCLEO_IHM03

namespace STM_ADC
{
    bool init()
    {
        // Set GPIO modes
        for (PinDef pin : IHM16M1ADC::PinDefs)
        {
            HAL_GPIO_Init(pin.port, &pin.init);
        }

        // Initialize ADC1
        if (__HAL_RCC_ADC12_IS_CLK_DISABLED())
            __HAL_RCC_ADC12_CLK_ENABLE();

        ADC_MultiModeTypeDef multimode = {0};

        hadc1.Instance 						= ADC1;
        hadc1.Init.ClockPrescaler 			= ADC_CLOCK_SYNC_PCLK_DIV4;
        hadc1.Init.Resolution 				= ADC_RESOLUTION_12B;
        hadc1.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
        hadc1.Init.GainCompensation 		= 0;
        hadc1.Init.ScanConvMode 			= ADC_SCAN_DISABLE;
        hadc1.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;
        hadc1.Init.LowPowerAutoWait 		= DISABLE;
        hadc1.Init.ContinuousConvMode 		= DISABLE;
        hadc1.Init.NbrOfConversion 			= 1;
        hadc1.Init.DiscontinuousConvMode 	= DISABLE;
        hadc1.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
        hadc1.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc1.Init.DMAContinuousRequests 	= DISABLE;
        hadc1.Init.Overrun 					= ADC_OVR_DATA_PRESERVED;
        hadc1.Init.OversamplingMode 		= DISABLE;

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
        hadc2.Instance 						= ADC2;
        hadc2.Init.ClockPrescaler 			= ADC_CLOCK_SYNC_PCLK_DIV4;
        hadc2.Init.Resolution 				= ADC_RESOLUTION_12B;
        hadc2.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
        hadc2.Init.GainCompensation 		= 0;
        hadc2.Init.ScanConvMode 			= ADC_SCAN_DISABLE;
        hadc2.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;
        hadc2.Init.LowPowerAutoWait 		= DISABLE;
        hadc2.Init.ContinuousConvMode 		= DISABLE;
        hadc2.Init.NbrOfConversion 			= 1;
        hadc2.Init.DiscontinuousConvMode 	= DISABLE;
        hadc2.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
        hadc2.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc2.Init.DMAContinuousRequests 	= DISABLE;
        hadc2.Init.Overrun 					= ADC_OVR_DATA_PRESERVED;
        hadc2.Init.OversamplingMode 		= DISABLE;

        if (HAL_ADC_Init(&hadc2) != HAL_OK)
        {
            return false;
        }

        // Configure ADCs
        ADC_ChannelConfTypeDef sConfig = {0};
        for (AdcDef adcDef : IHM16M1ADC::AdcDefs)
        {
            sConfig.Channel 		= adcDef.channel;
            sConfig.Rank 			= ADC_REGULAR_RANK_1;
            sConfig.SamplingTime 	= ADC_SAMPLETIME_2CYCLES_5;
            sConfig.SingleDiff 		= ADC_SINGLE_ENDED;
            sConfig.OffsetNumber 	= ADC_OFFSET_NONE;
            sConfig.Offset 			= 0;

            if (HAL_ADC_ConfigChannel(adcDef.adc == 1 ? &hadc1 : &hadc2, &sConfig) != HAL_OK)
            {
                return false;
            }
        }

        IHM16M1ADC::_mode = STM_ADC::Mode::Polling;
        IHM16M1ADC::_currentChannel = static_cast<IHM16M1ADC::PinNames>(0);

        // Enable interrupts
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
        
        // Initialization completed successfully
        return true;
    }

    void setMode(STM_ADC::Mode mode)
    {
        IHM16M1ADC::_mode = mode;
        
        // If the new mode is continuous conversion mode, kickstart the state machine
        HAL_ADC_Start_IT([] -> ADC_HandleTypeDef* {
            return &hadc1;
        });
    }

    void setRate(float rate)
    {

    }

    void setCallback(std::function<void(int)> callback)
    {
        
    }

    bool poll(int adc, int channel, STM_ADC::Conversion* result)
    {

    }

    bool pollAll(int adc)
    {

    }

    bool get(int adc, int channel, STM_ADC::Conversion* result)
    {

    }
}

// Handles the ADC state machine
void IHM16M1ADC::_adcConversionCallback()
{

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
    IHM16M1ADC::_adcConversionCallback();
}

#endif // PLATFORM_P_NUCLEO_IHM03