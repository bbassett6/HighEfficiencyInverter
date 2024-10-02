#include "impl/IHM16M1_inverter_impl.hpp"

#if PLATFORM_P_NUCLEO_IHM03

namespace Inverter
{
    bool init()
    {
        // Set GPIO modes for gate drive, current/voltage sense
        for (PinDef pin : IHM16M1Inverter::PinDefs)
        {
            HAL_GPIO_Init(pin.port, &pin.init);
        }

        // Enable Phases
        HAL_GPIO_WritePin(
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::En_U].port,
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::En_U].init.Pin,
            GPIO_PIN_SET
        );
        HAL_GPIO_WritePin(
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::En_V].port,
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::En_V].init.Pin,
            GPIO_PIN_SET
        );
        HAL_GPIO_WritePin(
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::En_W].port,
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::En_W].init.Pin,
            GPIO_PIN_SET
        );

        return true;
    }
    
    void setSwitchState(Inverter::SpaceVector vector)
    {
        Triple<bool> stateMap[Inverter::SpaceVector::NumSpaceVectors] = 
        {
            [Inverter::SpaceVector::V0] = {.U = 0, .V = 0, .W = 0},
            [Inverter::SpaceVector::V1] = {.U = 1, .V = 0, .W = 0},
            [Inverter::SpaceVector::V2] = {.U = 1, .V = 1, .W = 0},
            [Inverter::SpaceVector::V3] = {.U = 0, .V = 1, .W = 0},
            [Inverter::SpaceVector::V4] = {.U = 0, .V = 1, .W = 1},
            [Inverter::SpaceVector::V5] = {.U = 0, .V = 0, .W = 1},
            [Inverter::SpaceVector::V6] = {.U = 1, .V = 0, .W = 1},
            [Inverter::SpaceVector::V7] = {.U = 1, .V = 1, .W = 1}
        };
        HAL_GPIO_WritePin(
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::In_U].port,
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::In_U].init.Pin,
            (GPIO_PinState)stateMap[vector].U
        );
        HAL_GPIO_WritePin(
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::In_V].port,
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::In_V].init.Pin,
            (GPIO_PinState)stateMap[vector].V
        );
        HAL_GPIO_WritePin(
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::In_W].port,
            IHM16M1Inverter::PinDefs[IHM16M1Inverter::PinNames::In_W].init.Pin,
            (GPIO_PinState)stateMap[vector].W
        );
    }
    
    bool getPhaseCurrents(Triple<float>* currents)
    {
        // currents->U = (ADC->CH0-1.65)/0.75/0.03;
        // currents->V = (ADC->CH1-1.65)/0.75/0.03;
        // currents->W = (ADC->CH2-1.65)/0.75/0.03;
        // 30mV/A hall effect
        // 0.75V/V diff amp gain + 1.65 dc offset on output
    }
    
    bool getPhaseVoltages(Triple<float>* voltages)
    {

    }
    
    bool getBusVoltage(float* voltage)
    {
        // voltage = (ADC->CH3-1.65)/0.75*101;
        // HV = (voutp-m)*101
        // adc gives (voutp-m)0.075+1.65
    }
}

#endif // PLATFORM_P_NUCLEO_IHM03