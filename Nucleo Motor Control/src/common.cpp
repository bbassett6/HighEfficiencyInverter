#include "common.hpp"

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
    const bool zsmBClampEnabled     = true;
}

namespace RotorSenseParameters
{
    bool flipAngle            = false;
}

namespace PlatformParameters
{
    #if PLATFORM_P_NUCLEO_IHM03
    const float cpuMHz              = 170.0f;
    #elif PLATFORM_HEI
    const float cpuMHz              = 64.0f;
    #endif
}
namespace UARTParameters
{
    const int baudrate = 115200;
}

namespace SamplingParameters
{
    const float samplingRate = 100.0f;
}