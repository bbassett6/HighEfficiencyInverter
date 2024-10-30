#ifndef __SYM_PWM_INTERFACE
#define __SYM_PWM_INTERFACE

#include "common.hpp"
#include "types.hpp"

/*

SymmetricPWM Interface creates a common interface for generating centner-aligned 3 phase PWM 

*/

namespace SymmetricPWM
{
    bool init();
    void setPeriodNanos(unsigned int periodNanos);
    void setPhaseDuties(Vec3<float> phaseDuties);
}

#endif // __SYM_PWM_INTERFACE