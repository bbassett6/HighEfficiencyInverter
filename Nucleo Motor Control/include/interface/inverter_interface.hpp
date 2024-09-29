#ifndef __INVERTER_INTERFACE
#define __INVERTER_INTERFACE

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

    template <enum Implementation>
    class Interface
    {
        static bool init();
        static void setSwitchState(Inverter::SpaceVector vector);
        static bool getPhaseCurrents(Triple<float>* currents);      // Returns success
        static bool getPhaseVoltages(Triple<float>* voltages);      // Returns success
        static bool getBusVoltage(float* voltage);                  // Returns success
    };
}

#endif // __INVERTER_INTERFACE