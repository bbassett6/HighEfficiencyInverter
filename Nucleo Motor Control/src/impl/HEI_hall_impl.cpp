#include "impl/HEI_hall_impl.hpp"

#if PLATFORM_HEI

namespace Position
{
    float _offset = 0.0f;

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
            HEIHallSense::PinDefs[HEIHallSense::PinNames::Hall_A].port, 
            HEIHallSense::PinDefs[HEIHallSense::PinNames::Hall_A].init.Pin
        );
        GPIO_PinState hallBValue = HAL_GPIO_ReadPin(
            HEIHallSense::PinDefs[HEIHallSense::PinNames::Hall_B].port, 
            HEIHallSense::PinDefs[HEIHallSense::PinNames::Hall_B].init.Pin
        );
        GPIO_PinState hallCValue = HAL_GPIO_ReadPin(
            HEIHallSense::PinDefs[HEIHallSense::PinNames::Hall_C].port, 
            HEIHallSense::PinDefs[HEIHallSense::PinNames::Hall_C].init.Pin
        );

        float pos = HEIHallSense::AngleMap[hallAValue | (hallBValue << 1) | (hallCValue << 2)];

        // If all hall sensors are the same value, return invalid angle
        if (pos == -1.0f)
            return false;

        // invert angle if flipAngle is set
        if (flipAngle)
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
        
    }
}

#endif