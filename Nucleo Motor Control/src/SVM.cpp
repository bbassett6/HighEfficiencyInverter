#include "SVM.hpp"

namespace SVM
{   
    // Maps space vectors to nominal switch states
    static const Vec3<bool> SVSwitchMap[8] = 
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

    // Map space vectors to their Vec2 definitions
    static const Vec2<float>* VectorMap[8] =
    {
        [0] = NULL,
        [1] = &VEC_1,
        [2] = &VEC_2,
        [3] = &VEC_3,
        [4] = &VEC_4,
        [5] = &VEC_5,
        [6] = &VEC_6,
        [7] = NULL
    };

    // Maps sectors to space vector pairs
    // Sorted so .A's Hamming distance is closest to V0
    static const Vec2<int> SVSectorMap[6] =
    {
        [0] = {.u = {.A = 1, .B = 2}},
        [1] = {.u = {.A = 3, .B = 2}},
        [2] = {.u = {.A = 3, .B = 4}},
        [3] = {.u = {.A = 5, .B = 4}},
        [4] = {.u = {.A = 5, .B = 6}},
        [5] = {.u = {.A = 1, .B = 6}}
    };

    void init()
    {
        
    }

    // TODO: add dead time compensation. use current sense to determine direction of current through bridges
    // this will inform which body diode is active and thus the error caused by the HiZ state
    void setVecTarget(Vec2<float> newVecTarget)
    {
        Vec3<float> phaseDuties;
        float duty;
        int sector = 0;

        float angle = atan2f(newVecTarget.u.B, newVecTarget.u.A);
        if (angle < 0)
            angle += 2 * PI;

        sector = (int)(angle / (PI / 3));

        float Ax = VectorMap[SVSectorMap[sector].u.A]->u.A;
        float Ay = VectorMap[SVSectorMap[sector].u.A]->u.B;
        float Bx = VectorMap[SVSectorMap[sector].u.B]->u.A;
        float By = VectorMap[SVSectorMap[sector].u.B]->u.B;
        float Tx = newVecTarget.u.A;
        float Ty = newVecTarget.u.B;

        float determinant = 1.0f / (VectorMap[SVSectorMap[sector].u.A]->u.A * VectorMap[SVSectorMap[sector].u.B]->u.B - VectorMap[SVSectorMap[sector].u.B]->u.A * VectorMap[SVSectorMap[sector].u.A]->u.B);

        float stateDurationA = ((By * Tx) + (-Bx * Ty)) * determinant * 0.57735f;
        float stateDurationB =  ((-Ay * Tx) + (Ax * Ty)) * determinant * 0.57735f;

        // [N_0 / 2][SW_A / 2][SW_B / 2][N_7][SW_B / 2][SW_A / 2][N_0 / 2]
        // [N / 4][SW_A / 2][SW_B / 2][N / 2][SW_B / 2][SW_A / 2][N / 4]
        // N = 1.0 - SW_A - SW_B

        // Calculate duty cycle of the phase with the first rising edge
        // Then find that phase
        // [N / 4][SW_A / 2][SW_B / 2][N / 2][SW_B / 2][SW_A / 2][N / 4]
        //        |---------------------------------------------|
        // SW_A + SW_B + N/2
        // SW_A + SW_B + (1.0 - SW_A - SW_B) / 2
        // (SW_A + SW_B) / 2 + 0.5
        duty = 0.5f + ((stateDurationA + stateDurationB) / 2);
        for (int i = 0; i < Phase::NumPhases; i++)
        {
            if (SVSwitchMap[SVSectorMap[sector].u.A].a[i] == 1)
            {
                phaseDuties.a[i] = duty;
            }
        }
        
        // Calculate duty cycle of the phase with the second rising edge
        // Then find that phase
        // [N / 4][SW_A / 2][SW_B / 2][N / 2][SW_B / 2][SW_A / 2][N / 4]
        //                  |-------------------------|
        // SW_B + N/2
        // SW_B + (1.0 - SW_A - SW_B) / 2
        // 0.5 + (SW_B - SW_A) / 2
        duty = 0.5f + ((stateDurationB - stateDurationA) / 2);
        for (int i = 0; i < Phase::NumPhases; i++)
        {
            if ((SVSwitchMap[SVSectorMap[sector].u.A].a[i] ^ SVSwitchMap[SVSectorMap[sector].u.B].a[i]) == 1)
            {
                phaseDuties.a[i] = duty;
            }
        }

        // Calculate the duty cycle of the phase with the third rising edge
        // Then find that phase
        // [N / 4][SW_A / 2][SW_B / 2][N / 2][SW_B / 2][SW_A / 2][N / 4]
        //                            |-----|
        // N/2
        // (1.0 - SW_A - SW_B) / 2
        // 0.5 - (SW_A + SW_B) / 2
        duty = 0.5f - ((stateDurationA + stateDurationB) / 2);
        for (int i = 0; i < Phase::NumPhases; i++)
        {
            if ((SVSwitchMap[SVSectorMap[sector].u.A].a[i] | SVSwitchMap[SVSectorMap[sector].u.B].a[i]) == 0)
            {
                phaseDuties.a[i] = duty;
            }
        }

        // Apply ZSM bottom clamp if enabled
        if (PWMParameters::zsmBClampEnabled)
        {
            float zsmBottomClampOffset = 1.0f;
            for (int i = 0; i < Phase::NumPhases; i++)
            {
                if (phaseDuties.a[i] < zsmBottomClampOffset)
                    zsmBottomClampOffset = phaseDuties.a[i];
            }
            for (int i = 0; i < Phase::NumPhases; i++)
            {
                phaseDuties.a[i] -= zsmBottomClampOffset;
            }
        }

        SymmetricPWM::setPhaseDuties(phaseDuties);
    }
}