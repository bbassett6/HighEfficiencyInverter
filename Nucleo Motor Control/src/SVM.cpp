#include "SVM.hpp"
#include "interface/inverter_interface.hpp"
#include "interface/timer_interface.hpp"

namespace SVM
{
    void init()
    {
        
    }

    // TODO: add dead time compensation. use current sense to determine direction of current through bridges
    // this will inform which body diode is active and thus the error caused by the HiZ state
    void setVecTarget(Vec2<float> newVecTarget)
    {
        Vec3<float> phaseDuties;
        float duty;
        float largestDotProduct = 0.0;
        int sector = 0;

        // figure out the sector
        // take dot products with all 6 space vectors, find the largest 2
        float dotProducts[8] =
        {
            [0] = 0,
            [1] = dotProduct(&newVecTarget, &VEC_1),
            [2] = dotProduct(&newVecTarget, &VEC_2),
            [3] = dotProduct(&newVecTarget, &VEC_3),
            [4] = dotProduct(&newVecTarget, &VEC_4),
            [5] = dotProduct(&newVecTarget, &VEC_5),
            [6] = dotProduct(&newVecTarget, &VEC_6),
            [7] = 0
        };

        // find the sector with the largest dot product sum
        for (int i = 1; i < 7; i++)
        {
            float dotSum = dotProducts[i] + dotProducts[(i + 1) % 6];
            if (dotSum > largestDotProduct)
            {
                largestDotProduct = dotSum;
                sector = i - 1;
            }
        }

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
        duty = 0.5f + ((dotProducts[SVSectorMap[sector].u.A] + dotProducts[SVSectorMap[sector].u.B]) / 2);
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
        duty = 0.5f + ((dotProducts[SVSectorMap[sector].u.A] - dotProducts[SVSectorMap[sector].u.B]) / 2);
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
        // 0.5 - (SW_A + SW_B)
        duty = 0.5f - ((dotProducts[SVSectorMap[sector].u.A] + dotProducts[SVSectorMap[sector].u.B]) / 2);
        for (int i = 0; i < Phase::NumPhases; i++)
        {
            if ((SVSwitchMap[SVSectorMap[sector].u.A].a[i] | SVSwitchMap[SVSectorMap[sector].u.B].a[i]) == 0)
            {
                phaseDuties.a[i] = duty;
            }
        }

        SymmetricPWM::setPhaseDuties(phaseDuties);
    }
}