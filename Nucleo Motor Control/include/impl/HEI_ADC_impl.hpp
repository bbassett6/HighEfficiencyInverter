#ifndef __HEI_ADC_IMPL
#define __HEI_ADC_IMPL

#include "interface/adc_interface.hpp"
#include "interface/timer_interface.hpp"
#include "types.hpp"

#if PLATFORM_HEI

namespace HEIADC
{
    enum PinNames
    {
        I_Sense_A =     0,
        I_Sense_B =     1,
        I_Sense_C =     2,
        Vbus_Sense =    3,
        Ibus_Sense =    4,
        Therm_Sense_1 = 5,
        Therm_Sense_2 = 6,
        Therm_Sense_3 = 7,
        NumPins
    };

    const PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::I_Sense_A] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_0,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::I_Sense_B] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_1,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::I_Sense_C] =       {.port = GPIOA, .init = {.Pin = GPIO_PIN_2,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Vbus_Sense] =      {.port = GPIOA, .init = {.Pin = GPIO_PIN_5,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Ibus_Sense] =      {.port = GPIOA, .init = {.Pin = GPIO_PIN_4,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Therm_Sense_1] =   {.port = GPIOA, .init = {.Pin = GPIO_PIN_6,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Therm_Sense_2] =   {.port = GPIOA, .init = {.Pin = GPIO_PIN_7,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Therm_Sense_3] =   {.port = GPIOB, .init = {.Pin = GPIO_PIN_2,   .Mode = GPIO_MODE_ANALOG,       .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
    };

    const AdcDef AdcDefs[PinNames::NumPins] =
    {
        [PinNames::I_Sense_A] =     {.adc = 1,  .channel = ADC_CHANNEL_1},
        [PinNames::I_Sense_B] =     {.adc = 1,  .channel = ADC_CHANNEL_2},
        [PinNames::I_Sense_C] =     {.adc = 1,  .channel = ADC_CHANNEL_1},
        [PinNames::Vbus_Sense] =    {.adc = 2,  .channel = ADC_CHANNEL_2},
        [PinNames::Ibus_Sense] =    {.adc = 2,  .channel = ADC_CHANNEL_1},
        [PinNames::Therm_Sense_1] = {.adc = 2,  .channel = ADC_CHANNEL_3},
        [PinNames::Therm_Sense_2] = {.adc = 2,  .channel = ADC_CHANNEL_4},
        [PinNames::Therm_Sense_3] = {.adc = 2,  .channel = ADC_CHANNEL_12},
    };
}

#endif // PLATFORM_HEI
#endif // __HEI_ADC_IMPL