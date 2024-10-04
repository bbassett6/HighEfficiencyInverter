#ifndef __HEI_HALL_SENSE
#define __HEI_HALL_SENSE

#include "common.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "interface/position_interface.hpp"

#if PLATFORM_HEI

namespace HEIHallSense
{
    enum PinNames
    {
        Hall_A =    0,
        Hall_B =    1,
        Hall_C =    2,
        NumPins
    };

    // TODO: properly define these pins
    const PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::Hall_A] =    {.port = GPIOA, .init = {.Pin = GPIO_PIN_0,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Hall_B] =    {.port = GPIOA, .init = {.Pin = GPIO_PIN_0,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Hall_C] =    {.port = GPIOA, .init = {.Pin = GPIO_PIN_0,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}}
    };
}

#endif  // PLATFORM_HEI
#endif  // __HEI_HALL_SENSE