#ifndef __IHM16M1_ADC_IMPL
#define __IHM16M1_ADC_IMPL

#include "interface/adc_interface.hpp"
#include "types.hpp"

#if PLATFORM_P_NUCLEO_IHM03

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;


namespace IHM16M1ADC
{
    enum PinNames
    {
        I_Sense_U =     0,
        I_Sense_V =     1,
        I_Sense_W =     2,
        Vbus_Sense =    3,
        NumPins
    };

    const PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::I_Sense_U] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_1,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::I_Sense_V] =       {.port = GPIOB, .init = {.Pin = GPIO_PIN_11,  .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::I_Sense_W] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_7,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Vbus_Sense] =      {.port = GPIOA, .init = {.Pin = GPIO_PIN_0,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
    };

    const AdcDef AdcDefs[PinNames::NumPins] =
    {
        [PinNames::I_Sense_U] =     {.adc = 1,  .channel = ADC_CHANNEL_1},
        [PinNames::I_Sense_V] =     {.adc = 1,  .channel = ADC_CHANNEL_2},
        [PinNames::I_Sense_W] =     {.adc = 2,  .channel = ADC_CHANNEL_4},
        [PinNames::Vbus_Sense] =    {.adc = 2,  .channel = ADC_CHANNEL_14}
    };

    enum State
    {
        Idle =                  0,
        PollSingleBlocking =    1,
        PollAllBlocking =       2,
        PollSingleIT =          3,
        PollAllIT =             4,
        Continuous =            5,
        NumActions
    };

    State _currentAction =          State::Idle;
    STM_ADC::Mode _mode =           STM_ADC::Mode::Polling;
    PinNames _currentChannel =      static_cast<PinNames>(0);   // Used to keep track of which channel is currently being sampled
    float _rate =                   20000;                      // Sampling rate in hertz
    
    void _adcConversionCallback();                              // automatically called when conversion finishes. Advances polling state
}

#endif // PLATFORM_P_NUCLEO_IHM03
#endif // __IHM16M1_ADC_IMPL