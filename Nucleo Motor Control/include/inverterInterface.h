#ifndef __INVERTER_INTERFACE
#define __INVERTER_INTERFACE

#include "types.h"

/*

InverterInterface creates a common interface used by ControlSystem
It is abstract and may be implemented differently for different hardware

*/

namespace Inverter
{
    enum SpaceVector
    {
        V0,
        V1,
        V2,
        V3,
        V4,
        V5,
        V6,
        V7,
        NumSpaceVectors
    };

    class Interface
    {
        virtual void setSwitchState(Inverter::SpaceVector vector);
        virtual bool getPhaseCurrents(Float3* currents);            // Returns success
        virtual bool getPhaseVoltages(Float3* voltages);            // Returns success
        virtual bool getBusVoltage(float* voltage);                 // Returns success
    };
}

#endif // __INVERTER_INTERFACE