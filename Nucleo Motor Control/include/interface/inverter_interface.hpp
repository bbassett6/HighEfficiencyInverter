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
    bool init(void);
    bool getPhaseCurrents(Vec3<float>* currents);      // Returns success
    bool getPhaseVoltages(Vec3<float>* voltages);      // Returns success
    bool getBusVoltage(float* voltage);                  // Returns success
}

#endif // __INVERTER_INTERFACE