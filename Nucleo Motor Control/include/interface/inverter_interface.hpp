#ifndef __INVERTER_INTERFACE
#define __INVERTER_INTERFACE

#include "common.hpp"
#include "types.hpp"

/*

InverterInterface creates a common interface used by ControlSystem
It is abstract and may be implemented differently for different hardware

*/

namespace Inverter
{
    enum BridgeState
    {
        HiZ =   0,
        Low =   1,
        High =  2,
        NumStates
    };

    bool init(void);
    void setBridgeState(Triple<BridgeState> states);
    const Triple<BridgeState>& getBridgeState(void);
    bool getPhaseCurrents(Triple<float>* currents);      // Returns success
    bool getPhaseVoltages(Triple<float>* voltages);      // Returns success
    bool getBusVoltage(float* voltage);                  // Returns success
}

#endif // __INVERTER_INTERFACE