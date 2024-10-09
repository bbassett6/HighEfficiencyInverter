#include "SVM.hpp"
#include "interface/inverter_interface.hpp"
#include "interface/timer_interface.hpp"

namespace SVM
{
    void advanceStateMachine()
    {
        // check if state durations have been calculated
        if (!_initialized)
        {
            updateStateDurations();
            _initialized = true;
        }
        // if they have, advance the state
        else
        {
            _state = _nextState;
            _nextState = static_cast<PhyState>((_state + 1) % PhyState::NumStates);
        }

        // if in a null state, allow vecTarget to be updated
        if (_state == PhyState::Null0 || _state == PhyState::Null6)
        {
            if (_pendingVecTarget)
            {
                _vecTarget = _newVecTarget;

                _pendingVecTarget = false;
            }
        }

        // TODO: set the period of the timer using the current state's calculated duration
    }

    void updateStateDurations()
    {
        unsigned long timeBudget = switchingPeriodNanos - (deadtimeNanos * 6); // there are 6 dead periods
        float largestDotProduct = 0.0;
        int sector = 0;

        // figure out the sector
        // take dot products with all 6 space vectors, find the largest 2
        float dotProducts[6] =
        {
            [0] = dotProduct(_vecTarget, VEC_1),
            [1] = dotProduct(_vecTarget, VEC_2),
            [2] = dotProduct(_vecTarget, VEC_3),
            [3] = dotProduct(_vecTarget, VEC_4),
            [4] = dotProduct(_vecTarget, VEC_5),
            [5] = dotProduct(_vecTarget, VEC_6)
        };

        // find the sector with the largest dot product sum
        for (int i = 0; i < 6; i++)
        {
            float dotSum = dotProducts[i] + dotProducts[(i + 1) % 6];
            if (dotSum > largestDotProduct)
            {
                largestDotProduct = dotSum;
                sector = i;
            }
        }

        // use sector map to assign sector vectors
        _CWVector = SVSectorMap[sector].A;
        _CCWVector = SVSectorMap[sector].B;

        // use computed dot products to determine switching time
        unsigned long CWCycles = (timeBudget / 2) * dotProducts[sector];
        unsigned long CCWCycles = (timeBudget / 2) * dotProducts[(sector + 1) % 6];

        timeBudget -= CWCycles + CCWCycles;

        // assign computed state durations
        // null vectors
        _stateDurationsNanos[PhyState::Null0] = timeBudget / 2;
        _stateDurationsNanos[PhyState::Null6] = timeBudget - _stateDurationsNanos[PhyState::Null0];
        // CW vectors
        _stateDurationsNanos[PhyState::SW2] = CWCycles / 2;
        _stateDurationsNanos[PhyState::SW10] = CWCycles - _stateDurationsNanos[PhyState::SW2];
        // CCW vectors
        _stateDurationsNanos[PhyState::SW4] = CWCycles / 2;
        _stateDurationsNanos[PhyState::SW8] = CWCycles - _stateDurationsNanos[PhyState::SW4];
    }

    void setVecTarget(Vec2<float> newVecTarget)
    {
        _newVecTarget = newVecTarget;
        _pendingVecTarget = true;
    }
}