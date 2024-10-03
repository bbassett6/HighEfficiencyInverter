#ifndef __IHM16M1_IMPL
#define __IHM16M1_IMPL

#include "stm32g4xx_hal.h"
#include "parameters.h"
#include "types.hpp"
#include "interface/inverter_interface.hpp"
#include "interface/adc_interface.hpp"

#if PLATFORM_P_NUCLEO_IHM03

namespace IHM16M1Inverter
{
    enum PinNames
    {
        En_U =          0,
        En_V =          1,
        En_W =          2,
        In_U =          3,
        In_V =          4,
        In_W =          5,
        En_Fault =      6,
        NumPins
    };

    const PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::En_U] =            {.port = GPIOB, .init = {.Pin = GPIO_PIN_13,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::En_V] =            {.port = GPIOB, .init = {.Pin = GPIO_PIN_14,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::En_W] =            {.port = GPIOB, .init = {.Pin = GPIO_PIN_15,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::In_U] =            {.port = GPIOA, .init = {.Pin = GPIO_PIN_8,   .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::In_V] =            {.port = GPIOA, .init = {.Pin = GPIO_PIN_9,   .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::In_W] =            {.port = GPIOA, .init = {.Pin = GPIO_PIN_10,  .Mode = GPIO_MODE_OUTPUT_PP,    .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::En_Fault] =        {.port = GPIOA, .init = {.Pin = GPIO_PIN_11,  .Mode = GPIO_MODE_INPUT,        .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}}
    };
}

#endif // PLATFORM_P_NUCLEO_IHM03
#endif // __IHM16M1_IMPL