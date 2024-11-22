#include "interface/adc_interface.hpp"
#include "interface/timer_interface.hpp"
#include "types.hpp"
#include <tuple>

#if PLATFORM_HEI

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

namespace STM_ADC
{
    enum PinNames
    {
        I_Sense_A =     0,
        I_Sense_B =     1,
        I_Sense_C =     2,
        Ibus_Sense =    3,
        Vbus_Sense =    4,
        Therm_Sense_1 = 5,
        Therm_Sense_2 = 6,
        Therm_Sense_3 = 7,
        NumPins
    };

    static const PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::I_Sense_A] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_0,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::I_Sense_B] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_1,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::I_Sense_C] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_2,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Ibus_Sense] =      {.port = GPIOA, .init = {.Pin = GPIO_PIN_4,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Vbus_Sense] =      {.port = GPIOA, .init = {.Pin = GPIO_PIN_5,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Therm_Sense_1] =   {.port = GPIOA, .init = {.Pin = GPIO_PIN_6,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Therm_Sense_2] =   {.port = GPIOA, .init = {.Pin = GPIO_PIN_7,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Therm_Sense_3] =   {.port = GPIOB, .init = {.Pin = GPIO_PIN_2,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
    };

    // channel, rank, adc
    static const std::tuple<int, int, ADC_HandleTypeDef*> AdcChannels[PinNames::NumPins] =
    {
        [PinNames::I_Sense_A] =     {ADC_CHANNEL_1,  ADC_REGULAR_RANK_1, &hadc1},
        [PinNames::I_Sense_B] =     {ADC_CHANNEL_2,  ADC_REGULAR_RANK_2, &hadc1},
        [PinNames::I_Sense_C] =     {ADC_CHANNEL_3,  ADC_REGULAR_RANK_3, &hadc1},
        [PinNames::Ibus_Sense] =    {ADC_CHANNEL_1,  ADC_REGULAR_RANK_1, &hadc2},
        [PinNames::Vbus_Sense] =    {ADC_CHANNEL_2,  ADC_REGULAR_RANK_2, &hadc2},
        [PinNames::Therm_Sense_1] = {ADC_CHANNEL_3,  ADC_REGULAR_RANK_3, &hadc2},
        [PinNames::Therm_Sense_2] = {ADC_CHANNEL_4,  ADC_REGULAR_RANK_4, &hadc2},
        [PinNames::Therm_Sense_3] = {ADC_CHANNEL_12, ADC_REGULAR_RANK_5, &hadc2},
    };

    uint32_t _results[PinNames::NumPins] = {0};

    bool _initialized = false;
    unsigned long _timerPeriodNanos = 0;
    std::function<void(uint32_t*, int)> _userCallback; // _userCallback(HEIADC::PinNames channel, int result)
    bool _adc1Finished = false;
    bool _adc2Finished = false;

    // Configure ADC parameters here
    ADC_ChannelConfTypeDef _channelConfig = {
        .Channel        = 0, // nonsense value, must be set before _channelConfig can be used
        .Rank 			= ADC_REGULAR_RANK_1,
        .SamplingTime 	= ADC_SAMPLETIME_2CYCLES_5,
        .SingleDiff 	= ADC_SINGLE_ENDED,
        .OffsetNumber 	= ADC_OFFSET_NONE,
        .Offset 		= 0,
    };

    // restarts sampling
    void _poke()
    {
        HAL_ADC_Start_DMA(&hadc1, _results + 0, 3);
        HAL_ADC_Start_DMA(&hadc2, _results + 3, 5);
    }

    bool init()
    {
        // Enable DMA
        __HAL_RCC_DMA1_CLK_ENABLE();
        HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
        HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

        // DMA channel 1 config
        hdma_adc1.Instance                  = DMA1_Channel1;
        hdma_adc1.Init.Direction            = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc            = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc               = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment  = DMA_PDATAALIGN_HALFWORD;
        hdma_adc1.Init.MemDataAlignment     = DMA_MDATAALIGN_WORD;
        hdma_adc1.Init.Mode                 = DMA_NORMAL;
        hdma_adc1.Init.Priority             = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        {
            return false;
        }
        __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

        // DMA channel 2 config
        hdma_adc2.Instance                  = DMA1_Channel2;
        hdma_adc2.Init.Direction            = DMA_PERIPH_TO_MEMORY;
        hdma_adc2.Init.PeriphInc            = DMA_PINC_DISABLE;
        hdma_adc2.Init.MemInc               = DMA_MINC_ENABLE;
        hdma_adc2.Init.PeriphDataAlignment  = DMA_PDATAALIGN_HALFWORD;
        hdma_adc2.Init.MemDataAlignment     = DMA_MDATAALIGN_WORD;
        hdma_adc2.Init.Mode                 = DMA_NORMAL;
        hdma_adc2.Init.Priority             = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
        {
            return false;
        }
        __HAL_LINKDMA(&hadc2,DMA_Handle,hdma_adc2);

        // Set GPIO modes
        for (PinDef pin : PinDefs)
        {
            HAL_GPIO_Init(pin.port, &pin.init);
        }

        // Initialize ADC1
        if (__HAL_RCC_ADC12_IS_CLK_DISABLED())
            __HAL_RCC_ADC12_CLK_ENABLE();
        
        ADC_MultiModeTypeDef multimode = {0};

        hadc1.Instance                      = ADC1;
        hadc1.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
        hadc1.Init.Resolution               = ADC_RESOLUTION_12B;
        hadc1.Init.ScanConvMode             = ADC_SCAN_ENABLE;
        hadc1.Init.ContinuousConvMode       = ENABLE;
        hadc1.Init.DiscontinuousConvMode    = DISABLE;
        hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc1.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
        hadc1.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
        hadc1.Init.NbrOfConversion          = 3;
        hadc1.Init.DMAContinuousRequests    = DISABLE;
        hadc1.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
        hadc1.Init.LowPowerAutoWait         = DISABLE;
        hadc1.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;
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
        hadc2.Instance                      = ADC2;
        hadc2.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
        hadc2.Init.Resolution               = ADC_RESOLUTION_12B;
        hadc2.Init.ScanConvMode             = ADC_SCAN_ENABLE;
        hadc2.Init.ContinuousConvMode       = ENABLE;
        hadc2.Init.DiscontinuousConvMode    = DISABLE;
        hadc2.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc2.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
        hadc2.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
        hadc2.Init.NbrOfConversion          = 5;
        hadc2.Init.DMAContinuousRequests    = DISABLE;
        hadc2.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
        hadc2.Init.LowPowerAutoWait         = DISABLE;
        hadc2.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;
        if (HAL_ADC_Init(&hadc2) != HAL_OK)
        {
            return false;
        }

        // Configure ADCs
        ADC_ChannelConfTypeDef sConfig = {0};
        for (std::tuple<int, int, ADC_HandleTypeDef*> adcDef : AdcChannels)
        {
            sConfig.Channel         = std::get<0>(adcDef);
            sConfig.Rank            = std::get<1>(adcDef);
            ADC_HandleTypeDef* adc  = std::get<2>(adcDef);

            if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
            {
                return false;
            }
        }

        if (!(HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) == HAL_OK)) {
            return false;
        }
        if (!(HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) == HAL_OK)) {
            return false;
        }

        // // Enable interrupts
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

        _initialized = true;

        STM_TIMER::registerCallback(3, _poke);
        setRate(SamplingParameters::samplingRate);
        
        // Initialization completed successfully
        return true;
    }

    void setRate(float rate)
    {
        _timerPeriodNanos = 1e9 / rate;

        STM_TIMER::setPeriod(3, _timerPeriodNanos);
    }

    void setCallback(std::function<void(uint32_t*, int)> callback)
    {
        _userCallback = callback;
    }

    // Handles the ADC state machine
    void _adcConversionCallback(ADC_HandleTypeDef* hadc)
    {
        if (_initialized)
        {
            if (hadc == &hadc1)
                _adc1Finished = true;
            if (hadc == &hadc2)
                _adc2Finished = true;
            if (_adc1Finished && _adc2Finished && _userCallback)
            {
                _userCallback(_results, PinNames::NumPins);
                _adc1Finished = false;
                _adc2Finished = false;
            }
        }
    }
}

// Configure interrupt handlers for ADC1 and ADC2
extern "C" void ADC1_2_IRQHandler()
{
    HAL_ADC_IRQHandler(&hadc1);
    HAL_ADC_IRQHandler(&hadc2);
}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc1);
}

extern "C" void DMA1_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc2);
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    STM_ADC::_adcConversionCallback(hadc);
}

#endif // PLATFORM_HEI