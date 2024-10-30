#include "vecmath.hpp"

float dotProduct(Vec2<float> v1, Vec2<float> v2)
{
    return (v1.u.A * v2.u.A) + (v1.u.B * v2.u.B);
}

// bool clarkeTransform(Vec2<float>* clarkeCurrents)
// {
//     clarkeCurrents->u.A = currents.u.A;
//     clarkeCurrents->u.B = currents.u.A*.57735 + currents.u.B*1.1547;
// }