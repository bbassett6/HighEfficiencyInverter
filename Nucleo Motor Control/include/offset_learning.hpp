#ifndef __OFFSET_LEARNING
#define __OFFSET_LEARNING

#include <math.h>
#include "common.hpp"
#include "constants.hpp"
#include "interface/position_interface.hpp"
#include "SVM.hpp"

namespace OffsetLearning
{
    const float openLoopPower = 0.3;
    const float openLoopLearningTime = 5.0f; // time in seconds for offset learning to take
    bool learnOffset();
}

#endif // __OFFSET_LEARNING