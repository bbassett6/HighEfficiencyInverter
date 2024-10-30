#ifndef __VEC_MATH
#define __VEC_MATH

#include "types.hpp"

float dotProduct(const Vec2<float>* v1, const Vec2<float>* v2);
Vec2<float> clarkeTransform(const Vec3<float>* input);
// Vec2<float> parkTransform(const Vec2<float>* input, float theta);

#endif // __VEC_MATH