#ifndef __IHM16M1_ADC_IMPL
#define __IHM16M1_ADC_IMPL

#include "stm32g4xx_hal.h"
#include "parameters.h"
#include "peripherals.h"
#include "types.hpp"
#include "interface/adc_interface.hpp"

#if PLATFORM_P_NUCLEO_IHM03

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

    class Instance : STM_ADC::Interface<Implementation::IHM161M1>
    {
    private:
        static STM_ADC::Mode _mode;
        static PinNames _currentChannel;    // Used to keep track of which channel is currently being sampled
    public:
        static bool init();
        static void setMode(STM_ADC::Mode mode);
        static void setRate(int rate);                                          // set number of automatic samples per second
        static void setCallback(std::function<void(int)> callback);             // configure a callback when automatic sampling completes
        static bool poll(int adc, int channel, STM_ADC::Conversion* result);    // triggers a blocking conversion and returns the result
        static bool pollAll(int adc);                                           // triggers blocking conversions on all configured channels
        static bool get(int adc, int channel, STM_ADC::Conversion* result);     // gets the latest conversion from a specified channel
        static void _adcConversionCallback();                                   // automatically called when conversion finishes. Advances polling state
    };
}

#endif // PLATFORM_P_NUCLEO_IHM03
#endif // __IHM16M1_ADC_IMPL