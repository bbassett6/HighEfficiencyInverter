#include "vecmath.hpp"

float dotProduct(Vec2<float> v1, Vec2<float> v2)
{
    return (v1.A * v2.A) + (v1.B * v2.B);
}