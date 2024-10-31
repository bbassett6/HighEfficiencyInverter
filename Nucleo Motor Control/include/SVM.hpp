#ifndef __SVM
#define __SVM

#include "common.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "vecmath.hpp"
#include "interface/symmetric_pwm_interface.hpp"

namespace SVM
{
    // Maps space vectors to nominal switch states
    const Vec3<bool> SVSwitchMap[8] = 
    {
        [0] = {.u = {.A = 0,    .B= 0,    .C = 0}},
        [1] = {.u = {.A = 1,    .B= 0,    .C = 0}},
        [2] = {.u = {.A = 1,    .B= 1,    .C = 0}},
        [3] = {.u = {.A = 0,    .B= 1,    .C = 0}},
        [4] = {.u = {.A = 0,    .B= 1,    .C = 1}},
        [5] = {.u = {.A = 0,    .B= 0,    .C = 1}},
        [6] = {.u = {.A = 1,    .B= 0,    .C = 1}},
        [7] = {.u = {.A = 1,    .B= 1,    .C = 1}}
    };

    // Maps sectors to space vector pairs
    // Sorted so .A's Hamming distance is closest to V0
    const Vec2<int> SVSectorMap[6] =
    {
        [0] = {.u = {.A = 1, .B = 2}},
        [1] = {.u = {.A = 3, .B = 2}},
        [2] = {.u = {.A = 3, .B = 4}},
        [3] = {.u = {.A = 5, .B = 4}},
        [4] = {.u = {.A = 5, .B = 6}},
        [5] = {.u = {.A = 1, .B = 6}}
    };

    void init();
    void setVecTarget(Vec2<float> newVecTarget);      // set the SVM voltage vector, calculates new duty cycles
}

#endif  // __SVM