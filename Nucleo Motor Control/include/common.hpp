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
    extern const float phaseResistance;
    extern const float phaseInductance;
    extern const int polePairs;
    extern const float rotorOffset;
    extern const bool flipMotor;
}

namespace PWMParameters
{
    extern const int switchingPeriodNanos;
    extern const int deadtimeCyclesRising;
    extern const int deadtimeCyclesFalling;
    extern const bool zsmBClampEnabled;
}

namespace RotorSenseParameters
{
    extern bool flipAngle;
}

namespace PlatformParameters
{
    #if PLATFORM_P_NUCLEO_IHM03
    extern const float cpuMHz;
    #elif PLATFORM_HEI
    extern const float cpuMHz;
    #endif
}
namespace UARTParameters
{
    extern const int baudrate;
}

namespace SamplingParameters
{
    extern const float samplingRate;
}

#endif // __PARAMETERS