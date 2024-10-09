#include "impl/HEI_inverter_impl.hpp"

#if PLATFORM_HEI

namespace Inverter
{
    Triple<Inverter::BridgeState> _bridgeStates =
    {
        .A = Inverter::BridgeState::HiZ,
        .B = Inverter::BridgeState::HiZ,
        .C = Inverter::BridgeState::HiZ,
    };

    bool init()
    {
        // Set GPIO modes for gate drive, current/voltage sense
        for (PinDef pin : HEIInverter::PinDefs)
        {
            HAL_GPIO_Init(pin.port, &pin.init);
        }

        // Set all phases to HiZ
        // Set high-sides high and low-sides low
        HAL_GPIO_WritePin(
            HEIInverter::PinDefs[HEIInverter::PinNames::A_HS].port, 
            HEIInverter::PinDefs[HEIInverter::PinNames::A_HS].init.Pin, 
            GPIO_PIN_SET
        );
        HAL_GPIO_WritePin(
            HEIInverter::PinDefs[HEIInverter::PinNames::B_HS].port, 
            HEIInverter::PinDefs[HEIInverter::PinNames::B_HS].init.Pin, 
            GPIO_PIN_SET
        );
        HAL_GPIO_WritePin(
            HEIInverter::PinDefs[HEIInverter::PinNames::C_HS].port, 
            HEIInverter::PinDefs[HEIInverter::PinNames::C_HS].init.Pin, 
            GPIO_PIN_SET
        );
        HAL_GPIO_WritePin(
            HEIInverter::PinDefs[HEIInverter::PinNames::A_LS].port, 
            HEIInverter::PinDefs[HEIInverter::PinNames::A_LS].init.Pin, 
            GPIO_PIN_RESET
        );
        HAL_GPIO_WritePin(
            HEIInverter::PinDefs[HEIInverter::PinNames::B_LS].port, 
            HEIInverter::PinDefs[HEIInverter::PinNames::B_LS].init.Pin, 
            GPIO_PIN_RESET
        );
        HAL_GPIO_WritePin(
            HEIInverter::PinDefs[HEIInverter::PinNames::C_LS].port, 
            HEIInverter::PinDefs[HEIInverter::PinNames::C_LS].init.Pin, 
            GPIO_PIN_RESET
        );

        return true;
    }
    
    void setSwitchState(Triple<BridgeState> states)
    {
        _bridgeStates = states;

        for (int i = 0; i < HEIInverter::NumPins; i++)
        {
            const PinDef* pinDef = &HEIInverter::PinDefs[i]; // Get the pinDef for this FET
            BridgeState bridgeState;

            // Fetch the state for this bridge
            if (i == HEIInverter::PinNames::A_HS || i == HEIInverter::PinNames::A_LS)
                bridgeState = states.A;
            else if (i == HEIInverter::PinNames::B_HS || i == HEIInverter::PinNames::B_LS)
                bridgeState = states.B;
            else
                bridgeState = states.C;

            GPIO_PinState pinState;
            // Is the FET HS?
            // If yes, conduct (GPIO_PIN_RESET) when BridgeState::High
            if (i == HEIInverter::PinNames::A_HS || i == HEIInverter::PinNames::B_HS || i == HEIInverter::PinNames::C_HS)
                pinState = (bridgeState == BridgeState::High) ? GPIO_PIN_RESET : GPIO_PIN_SET;
            // FET is LS
            // Conduct (GPIO_PIN_SET) when BridgeState::Low
            else
                pinState = (bridgeState == BridgeState::Low) ? GPIO_PIN_SET : GPIO_PIN_RESET;

            // Set the FET
            HAL_GPIO_WritePin(
                pinDef->port, 
                pinDef->init.Pin, 
                pinState
            );
        }
    }
    
    bool getPhaseCurrents(Triple<float>* currents)
    {
        
    }
    
    bool getPhaseVoltages(Triple<float>* voltages)
    {

    }
    
    bool getBusVoltage(float* voltage)
    {
        
    }
}

#endif // PLATFORM_HEI