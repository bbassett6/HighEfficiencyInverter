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

    const PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::Hall_A] =    {.port = GPIOA, .init = {.Pin = GPIO_PIN_0,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Hall_B] =    {.port = GPIOA, .init = {.Pin = GPIO_PIN_1,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Hall_C] =    {.port = GPIOA, .init = {.Pin = GPIO_PIN_10,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}}
    };

    // Bit 0 is Hall A value
    // Bit 1 is Hall B value
    // Bit 2 us Hall C value
    const float AngleMap[8] =
    {
        [0b000] = -1.0f,
        [0b001] = 0.0f,
        [0b010] = 4 *  PI / 6,
        [0b011] = 2 *  PI / 6,
        [0b100] = 8 *  PI / 6,
        [0b101] = 10 * PI / 6,
        [0b110] = 6 *  PI / 6,
        [0b111] = -1.0f,
    };
}

#endif  // PLATFORM_HEI
#endif  // __HEI_HALL_SENSE