#ifndef __HEI_HALL_SENSE
#define __HEI_HALL_SENSE

#include "interface/position_interface.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "parameters.h"

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

    class Instance : Position::Interface<Implementation::HEI>
    {
        static void setOffset(float offset);       // This offset should be specified such that when the interface reports 0 angle, the rotor is electrically aligned with the alpha axis
        static bool getPosition(float* position);
        static bool getSpeed(float* speed);
    };
}

#endif  // PLATFORM_HEI
#endif  // __HEI_HALL_SENSE