#ifndef __CONSTANTS
#define __CONSTANTS

#include "types.hpp"

const float PI = 3.14159265;

// Coil vectors
const Vec2<float> VEC_A = {.A = 1.0,  .B = 0.0};
const Vec2<float> VEC_B = {.A = -0.5, .B = 0.707106781187};
const Vec2<float> VEC_C = {.A = -0.5, .B = -0.707106781187};

// Space vectors
const Vec2<float> VEC_1 = {.A = 1.0,  .B = 0.0};
const Vec2<float> VEC_2 = {.A = 0.5,  .B = 0.707106781187};
const Vec2<float> VEC_3 = {.A = -0.5, .B = 0.707106781187};
const Vec2<float> VEC_4 = {.A = -1.0, .B = 0.0};
const Vec2<float> VEC_5 = {.A = -0.5, .B = -0.707106781187};
const Vec2<float> VEC_6 = {.A = 0.5,  .B = -0.707106781187};

#endif // __CONSTANTS