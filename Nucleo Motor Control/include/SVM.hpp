#ifndef __SVM
#define __SVM

#include "common.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "vecmath.hpp"
#include "interface/symmetric_pwm_interface.hpp"
#include <math.h>

namespace SVM
{
    void init();
    void setVecTarget(Vec2<float> newVecTarget);      // set the SVM voltage vector, calculates new duty cycles
}

#endif  // __SVM