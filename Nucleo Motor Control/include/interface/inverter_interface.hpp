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
    enum SpaceVector
    {
        V0 = 0,
        V1 = 1,
        V2 = 2,
        V3 = 3,
        V4 = 4,
        V5 = 5,
        V6 = 6,
        V7 = 7,
        NumSpaceVectors
    };

    bool init();
    void setSwitchState(Inverter::SpaceVector vector);
    bool getPhaseCurrents(Triple<float>* currents);      // Returns success
    bool getPhaseVoltages(Triple<float>* voltages);      // Returns success
    bool getBusVoltage(float* voltage);                  // Returns success
}

#endif // __INVERTER_INTERFACE