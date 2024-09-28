#include "HEI_hall_impl.hpp"

namespace HEIHallSense
{
    class Instance : Position::Interface
    {
        void setOffset(float offset, ADC* ADC) 
        {
            
        }

        // TODO: use flipAngle from `constants.hpp` in case we want to reverse the detected angle
        bool getPosition(float* position, ADC* ADC)
        {
            GPIO_PinState hallAValue = HAL_GPIO_ReadPin(PinDefs[Hall_A].port, PinDefs[Hall_A].init.Pin);
            GPIO_PinState hallBValue = HAL_GPIO_ReadPin(PinDefs[Hall_B].port, PinDefs[Hall_B].init.Pin);
            GPIO_PinState hallCValue = HAL_GPIO_ReadPin(PinDefs[Hall_C].port, PinDefs[Hall_C].init.Pin);

            if (hallAValue == 1 && hallBValue == 0 && hallCValue == 0) {
                *position = 0.0f; // 0 radians
            } else if (hallAValue == 0 && hallBValue == 1 && hallCValue == 0) {
                *position = (2.0f * PI) / 3; // 120 degrees
            } else if (hallAValue == 0 && hallBValue == 0 && hallCValue == 1) {
                *position = (4.0f * PI) / 3; // 240 degrees
            } else if (hallAValue == 1 && hallBValue == 1 && hallCValue == 0) {
                *position = PI; // 180 degrees
            } else if (hallAValue == 1 && hallBValue == 0 && hallCValue == 1) {
                *position = (PI / 3); // 60 degrees
            } else if (hallAValue == 0 && hallBValue == 1 && hallCValue == 1) {
                *position = (5.0f * PI) / 3; // 300 degrees
            }

            return true; // TODO: if we can detect failure of a hall sensor, this should return false
        }

        bool getSpeed(float* speed, ADC* ADC) 
        {
            
        }
    };
}
