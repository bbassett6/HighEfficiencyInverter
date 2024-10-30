#include "vecmath.hpp"

float dotProduct(const Vec2<float>* v1, const Vec2<float>* v2)
{
    return (v1->u.A * v2->u.A) + (v1->u.B * v2->u.B);
}

Vec2<float> clarkeTransform(const Vec3<float>* input)
{
    return {.u = {.A = input->u.A, .B = input->u.A*.57735 + input->u.B*1.1547}};
}

