#include "common.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "interface/position_interface.hpp"

#if PLATFORM_HEI

namespace Position
{
    enum PinNames
    {
        Hall_A =    0,
        Hall_B =    1,
        Hall_C =    2,
        NumPins
    };

    const static PinDef PinDefs[PinNames::NumPins] = 
    {
        [PinNames::Hall_A] =    {.port = GPIOB, .init = {.Pin = GPIO_PIN_0,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Hall_B] =    {.port = GPIOB, .init = {.Pin = GPIO_PIN_1,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}},
        [PinNames::Hall_C] =    {.port = GPIOB, .init = {.Pin = GPIO_PIN_10,  .Mode = GPIO_MODE_INPUT,  .Pull = GPIO_NOPULL,    .Speed = GPIO_SPEED_FREQ_LOW,   .Alternate = 0}}
    };

    // Bit 0 is Hall A value
    // Bit 1 is Hall B value
    // Bit 2 us Hall C value
    const static float AngleMap[8] =
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

    // float _offset = 4.80475187f + (PI / 6.0f);
    float _offset = (3.0f * PI / 2.0f) + (PI / 6.0f);

    bool init()
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        for (int i = 0; i < PinNames::NumPins; i++)
        {
            GPIO_InitStruct = PinDefs[i].init;
            HAL_GPIO_Init(PinDefs[i].port, &GPIO_InitStruct);
        }

        return true;
    }

    void setOffset(float offset) 
    {
        // clamp to 2PI radians
        while (offset < 0.0f)
            offset += 2 * PI;
        while (offset >= 2 * PI)
            offset -= 2 * PI;
        _offset = offset;
    }

    bool getPosition(float* position)
    {
        GPIO_PinState hallAValue = HAL_GPIO_ReadPin(
            PinDefs[PinNames::Hall_A].port, 
            PinDefs[PinNames::Hall_A].init.Pin
        );
        GPIO_PinState hallBValue = HAL_GPIO_ReadPin(
            PinDefs[PinNames::Hall_B].port, 
            PinDefs[PinNames::Hall_B].init.Pin
        );
        GPIO_PinState hallCValue = HAL_GPIO_ReadPin(
            PinDefs[PinNames::Hall_C].port, 
            PinDefs[PinNames::Hall_C].init.Pin
        );

        float pos = AngleMap[hallAValue | (hallBValue << 1) | (hallCValue << 2)];

        // If all hall sensors are the same value, return invalid angle
        if (pos == -1.0f)
            return false;

        // invert angle if flipAngle is set
        if (RotorSenseParameters::flipAngle)
            pos = 2 * PI - pos;

        pos += _offset;

        // clamp angle to 2PI
        if (pos > 2 * PI)
            pos -= 2 * PI;

        *position = pos;

        return true;
    }

    bool getSpeed(float* speed) 
    {
        // TODO
    }
}

#endif