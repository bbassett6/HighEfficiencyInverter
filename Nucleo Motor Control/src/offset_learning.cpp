#include "offset_learning.hpp"

namespace OffsetLearning
{
    bool learnOffsetHelper()
    {
        Vec2<float> target;
        float offsets[6] = {0};
        int offsetIndex = 0;
        float angle;
        Position::getPosition(&angle);
        
        // Command the rotor to 0 electrical degrees
        target.u.A = cosf(0.0f) * openLoopPower;
        target.u.B = sinf(0.0f) * openLoopPower;
        SVM::setVecTarget(target);

        // Allow rotor to settle
        HAL_Delay(1000);

        // Slowly sweep across 2*PI radians and note when the hall effect system reports a new value
        for (int i = 0; i < openLoopLearningTime * 1000; i++)
        {
            float electricalAngle = i * 2.0f * PI / (openLoopLearningTime * 1000);

            target.u.A = cosf(electricalAngle) * openLoopPower;
            target.u.B = sinf(electricalAngle) * openLoopPower;
            SVM::setVecTarget(target);

            HAL_Delay(1);

            float newAngle = 0.0f;
            Position::getPosition(&newAngle);

            // check if the hall angle changed
            if (newAngle != angle)
            {
                // check if too many offsets have been recorded
                if (offsetIndex == 6)
                {
                    return false;
                }

                offsets[offsetIndex] = electricalAngle - newAngle;
                offsetIndex++;
                angle = newAngle;

                // skip the next few degrees to avoid retriggering
                electricalAngle += 5 * 2.0f * PI / (openLoopLearningTime * 1000);
                i += 5;
                target.u.A = cosf(electricalAngle) * openLoopPower;
                target.u.B = sinf(electricalAngle) * openLoopPower;
                SVM::setVecTarget(target);
                HAL_Delay(100);
            }
        }

        // check if the motor is backwards
        float avg = 0.0f;
        float dev = 0.0f;;

        for (int i = 0; i < 6; i++)
        {
            if (offsets[i] < 0)
                offsets[i] += 2 * PI;
            avg += offsets[i];
        }
        avg /= 6.0f;
        for (int i = 0; i < 6; i++)
        {
            dev += (avg - offsets[i]) * (avg - offsets[i]);
        }
        dev = sqrtf(dev);

        if (dev > 2.0f * PI / 360.0f * 10.0f)
        {
            return false;
        }

        // succeeded
        Position::setOffset(avg + (PI / 6.0f));

        return true;
    }

    // Performs the offset learning routine
    bool learnOffset()
    {
        if (!learnOffsetHelper())
        {
            // try flipping motor polarity and re-learn
            RotorSenseParameters::flipAngle = !RotorSenseParameters::flipAngle;
            SVM::setVecTarget({.u = {.A = 0.0f, .B = 0.0f}});
            return learnOffsetHelper();
        }

        SVM::setVecTarget({.u = {.A = 0.0f, .B = 0.0f}});
        return true;
    }
}