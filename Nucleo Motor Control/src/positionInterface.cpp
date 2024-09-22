#include "positionInterface.h"
#include "types.h"

#define HALL_A GPIO_PIN_0   // adjust these
#define HALL_B GPIO_PIN_1
#define HALL_C GPIO_PIN_2

namespace Position
{
    class Interface
    {
        void setOffset(float offset, ADC* ADC) 
        {
            
        }

        bool getPosition(float* position, ADC* ADC)
        {
            float pi = 3.14159265;
            GPIO_PinState hallAValue = HAL_GPIO_ReadPin(GPIOA, HALL_A_PIN);
            GPIO_PinState hallBValue = HAL_GPIO_ReadPin(GPIOA, HALL_B_PIN);
            GPIO_PinState hallCValue = HAL_GPIO_ReadPin(GPIOA, HALL_C_PIN);

            if (hallAValue == 1 && hallBValue == 0 && hallCValue == 0) {
                *position = 0.0f; // 0 radians
            } else if (hallAValue == 0 && hallBValue == 1 && hallCValue == 0) {
                *position = (2.0f * pi) / 3; // 120 degrees
            } else if (hallAValue == 0 && hallBValue == 0 && hallCValue == 1) {
                *position = (4.0f * pi) / 3; // 240 degrees
            } else if (hallAValue == 1 && hallBValue == 1 && hallCValue == 0) {
                *position = pi; // 180 degrees
            } else if (hallAValue == 1 && hallBValue == 0 && hallCValue == 1) {
                *position = (pi / 3); // 60 degrees
            } else if (hallAValue == 0 && hallBValue == 1 && hallCValue == 1) {
                *position = (5.0f * pi) / 3; // 300 degrees
            }
        }

        bool getSpeed(float* speed, ADC* ADC) 
        {
            
        }
    };
}