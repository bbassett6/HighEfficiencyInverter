#include "common.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "interface/position_interface.hpp"
#include "interface/timer_interface.hpp"
#include "platform_setup.hpp"
#include "interface/uart_interface.hpp"

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

    const float _timerFrequency = 2.0f;
    const float _predictionDiffThreshold = 2.0f * PI / 360.0f * 15.0f; // 5 degrees

    float _offset = 5.77257204f - (PI / 6.0f);
    // float _offset = (3.0f * PI / 2.0f) + (PI / 6.0f);

    // speed observation & position interpolation
    float _predictedAngle = 0.0f;
    float _observedSpeed = 0.0f;
    float _lastQuantizedAngle = -1.0f;
    volatile bool _speedObserverValid = false;

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
        
        // 2 Hz
        STM_TIMER::setFrequency(2, 2.0f);

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

        // Speed observer
        // Calculate predicted position
        // If quantized position has changed since last call to getPosition
        //      Set position to quantizedPosition
        //      Estimate speed based on timer value
        //      Reset timer
        //      If predicted position is reasonably close to the actual position
        //          Enable position interpolation
        // Else
        //     If predicted position exceeds some threshold, disable position interpolation
        float tim = STM_TIMER::hrTickGet();
        float predictionDiff = tim * _observedSpeed;
        _predictedAngle = _lastQuantizedAngle + predictionDiff;
        if (_predictedAngle >= 2.0f * PI)
            _predictedAngle -= 2.0f * PI;
        if (_predictedAngle < 0)
            _predictedAngle += 2.0f * PI;
        
        if (pos != _lastQuantizedAngle) 
        {
            if (_observedSpeed > 0.000000836f)
                _speedObserverValid = true;

            float quantDiff = pos - _lastQuantizedAngle;
            if (_observedSpeed > 0 && quantDiff < 0)
                quantDiff += 2.0f * PI;
            if (_observedSpeed < 0 && quantDiff > 0)
                quantDiff -= 2.0f * PI;

            if (tim > 50 && std::abs(quantDiff / tim) < 100.0f)
            {
                _lastQuantizedAngle = pos;
                _observedSpeed = (_observedSpeed * 0.5f) + (quantDiff / tim * 0.5f);
                STM_TIMER::hrTickReset();
            }

            float valid = _speedObserverValid ? 5.0f : 0.0f;
            // UART::transmit((unsigned char*)&_lastQuantizedAngle, 4);
            // UART::transmit((unsigned char*)&_observedSpeed, 4);
            // UART::transmit((unsigned char*)&valid, 4);
        }
        else
        {
            if (_observedSpeed > 0.0f && predictionDiff > (PI / 3.0f) + _predictionDiffThreshold)
                _speedObserverValid = false;
            if (_observedSpeed < 0.0f && predictionDiff < (-PI / 3.0f) - _predictionDiffThreshold)
                _speedObserverValid = false;
        }

        if (_speedObserverValid)
            *position = _predictedAngle;
        else
            *position = pos;

        // *position = pos;

        if (_speedObserverValid)
            HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);

        return true;
    }

    bool getSpeed(float* speed) 
    {
        // TODO
    }
}

#endif