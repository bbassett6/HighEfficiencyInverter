#ifndef __PARAMETERS
#define __PARAMETERS

#include <stdbool.h>

// Motor Parameters
const float phaseResistance     = 0.0575;
const float phaseInductance     = 20e-6;
const int polePairs             = 40;
const float rotorOffset         = 0.0;      // Added to raw electrical angle. Angle of 0 should indicate rotor is aligned with the alpha axis.
const bool flipMotor            = false;

// Switching Parameters
const int switchingFrequency    = 20000;
const int deadtimeMicros        = 1;
const float minSwitchingCycle   = 0.01;
const float maxSwitchingCycle   = 0.99;

#endif // __PARAMETERS