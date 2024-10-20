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
    
    void setBridgeState(Triple<BridgeState> states)
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

    const Triple<BridgeState>& getBridgeState(void)
    {
        return _bridgeStates;
    }

    bool getPhaseCurrents(Triple<float>* currents)
    {
        // not sure if this is how it will be implemented 
        //STM_ADC::pollAll(1); to get all data in conversion struct but not sure yet if that will be a list cache thing or what
        currents->A = (STM_ADC::Conversion::value)-1.65/0.75/0.03;
        currents->B = (STM_ADC::Conversion::value)-1.65/0.75/0.03;
        currents->C = (STM_ADC::Conversion::value)-1.65/0.75/0.03;
    }
    
    bool getPhaseVoltages(Triple<float>* voltages)
    {
        // TODO implement observer of some sort that calculates expected current from duty cycle and compares to average of current readings, 
        // need to be concious of the switching cycle and measure on an off and on pulse or multiple points on on and of to averag per switching cycle. 
        // 
        //
        // perform clarke transform to get A B to alpha beta currents
        // recreate phase voltages from DC bus and pwms
        // sliding mode based rotor position sensor uses i alpha and beta and expected phase voltages to estimate position
        getPhaseCurrents(Triple<float> currents);
        Vec2<float> clarkeCurrents;
    }

    bool clarkeTransform(Vec2<float>* clarkeCurrents)
    {
        clarkeCurrents->A = currents.A;
        clarkeCurrents->B = currents.A*.57735 + currents.B*1.1547;
    }

    bool getrotorPosition()
    {
        theta = -1*arctan(ea/eb);
    }
    
    bool getBusVoltage(float* voltage)
    {
        //STM_ADC::poll(2,2)
        currents->A = (STM_ADC::Conversion::value)-1.65/0.75*101;
    }
}
#endif // PLATFORM_HEI