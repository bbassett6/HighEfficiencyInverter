#ifndef __HEI_INVERTER_IMPL
#define __HEI_INVERTER_IMPL

#include "interface/inverter_interface.hpp"
#include "interface/adc_interface.hpp"
#include "types.hpp"

#if PLATFORM_HEI

namespace HEIInverter
{
    enum PinNames
    {
        A_HS =  0,
        A_LS =  1,
        B_HS =  2,
        B_LS =  3,
        C_HS =  4,
        C_LS =  5,
        NumPins
    };

    const PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::A_HS] =            {.port = GPIOA, .init = {.Pin = GPIO_PIN_11,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::A_LS] =            {.port = GPIOA, .init = {.Pin = GPIO_PIN_10,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::B_HS] =            {.port = GPIOA, .init = {.Pin = GPIO_PIN_9,   .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::B_LS] =            {.port = GPIOA, .init = {.Pin = GPIO_PIN_8,   .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::C_HS] =            {.port = GPIOB, .init = {.Pin = GPIO_PIN_15,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::C_LS] =            {.port = GPIOB, .init = {.Pin = GPIO_PIN_14,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}}
    };
}

#endif // PLATFORM_HEI
#endif // __HEI_INVERTER_IMPL