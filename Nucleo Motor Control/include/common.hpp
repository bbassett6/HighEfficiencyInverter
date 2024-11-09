#ifndef __PARAMETERS
#define __PARAMETERS

#include <stdbool.h>

// Platform Parameters
#define PLATFORM_HEI            1
#define PLATFORM_P_NUCLEO_IHM03 0

// Platform Includes
#if PLATFORM_P_NUCLEO_IHM03
#include "stm32g4xx.h"
#elif PLATFORM_HEI
#include "stm32f3xx.h"
#endif

namespace MotorParameters
{
    const float phaseResistance     = 0.0575;
    const float phaseInductance     = 20e-6;
    const int polePairs             = 40;
    const float rotorOffset         = 0.0;      // Added to raw electrical angle. Angle of 0 should indicate rotor is aligned with the alpha axis.
    const bool flipMotor            = false;
}

namespace PWMParameters
{
    const int switchingPeriodNanos  = 50000;    // 20kHz
    const int deadtimeCyclesRising  = 40;       // based on observed characteristics
    const int deadtimeCyclesFalling = 40;
    const bool zsmBClampEnabled     = false;
}

namespace RotorSenseParameters
{
    const bool flipAngle            = false;
}

namespace PlatformParameters
{
    #if PLATFORM_P_NUCLEO_IHM03
    const float cpuMHz              = 170.0f;
    #elif PLATFORM_HEI
    const float cpuMHz              = 64.0f;
    #endif
}

#endif // __PARAMETERS