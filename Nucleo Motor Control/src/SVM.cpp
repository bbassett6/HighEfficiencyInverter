#include "SVM.hpp"
#include "interface/inverter_interface.hpp"
#include "interface/timer_interface.hpp"

namespace SVM
{
    unsigned long _stateDurationsNanos[PhyState::NumStates] =
    {
        [PhyState::Null0] =   0,
        [PhyState::DT1] =     deadtimeNanos,
        [PhyState::SW2] =     0,
        [PhyState::DT3] =     deadtimeNanos,
        [PhyState::SW4] =     0,
        [PhyState::DT5] =     deadtimeNanos,
        [PhyState::Null6] =   0,
        [PhyState::DT7] =     deadtimeNanos,
        [PhyState::SW8] =     0,
        [PhyState::DT9] =     deadtimeNanos,
        [PhyState::SW10] =    0,
        [PhyState::DT11] =    deadtimeNanos
    };

    Triple<Inverter::BridgeState> _stateSequence[PhyState::NumStates] =
    {
        [PhyState::Null0] =   {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::DT1] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::SW2] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::DT3] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::SW4] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::DT5] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::Null6] =   {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::DT7] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::SW8] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::DT9] =     {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::SW10] =    {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ},
        [PhyState::DT11] =    {.A = Inverter::BridgeState::HiZ, .B = Inverter::BridgeState::HiZ, .C = Inverter::BridgeState::HiZ}
    };

    PhyState _state =   PhyState::Null0;                // SVM physical state machine
    PhyState _nextState = _state;
    int _CWVector =     1;                              // the clockwise-most vector for the current sector
    int _CCWVector =    2;                              // the counter-clockwise-most vector for the current sector
    Vec2<float> _vecTarget = {.A=0, .B=0};              // voltage vector the SVM state machine will produce
    bool _pendingVecTarget = false;                     // whether a new vecTarget has been requested
    Vec2<float> _newVecTarget = {.A=0, .B=0};           // the requested vecTarget
    bool _initialized = false;                          // whether _stateDurationsNanos has been calculated

    void init()
    {
        // TODO: bootstrap state machine. Set timer callback
    }
    
    void advanceStateMachine()
    {
        // check if calculations needed to switch have been performed
        if (!_initialized)
        {
            updateStateDurations();
            updateStateSequence();
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
                updateStateDurations();
                updateStateSequence();
            }
        }

        // Set inverter switch state
        Inverter::setSwitchState(_stateSequence[_state]);

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

    void updateStateSequence()
    {
        // determine switching sequence such that one bridge switches per cycle
        // start by inserting the known switch states
        _stateSequence[PhyState::SW2] =     SVSwitchMap[_CWVector];
        _stateSequence[PhyState::SW10] =    SVSwitchMap[_CWVector];
        _stateSequence[PhyState::SW4] =     SVSwitchMap[_CCWVector];
        _stateSequence[PhyState::SW8] =     SVSwitchMap[_CCWVector];

        Triple<Inverter::BridgeState> nearestNullToCW;
        Triple<Inverter::BridgeState> nearestNullToCCW;

        // first determine which null state is closest to SW2 & SW10 (CW)
        // do this by summing the number of "high" bridges
        // if it's 1, V0 is closest
        // if it's 2, V7 is closest
        // summing can be done by XOR'ing all bridge states, e.g.,
        // 1 ^ 1 ^ 0 = 0
        // 1 ^ 0 ^ 0 = 1
        // this works because CW and CCW vectors will NOT be null states
        if (
            (_stateSequence[PhyState::SW2].A == Inverter::BridgeState::High) ^
            (_stateSequence[PhyState::SW2].B == Inverter::BridgeState::High) ^
            (_stateSequence[PhyState::SW2].C == Inverter::BridgeState::High)
        )
        {
            // odd # of high switches (1 high switch)
            nearestNullToCW = SVSwitchMap[0];
            nearestNullToCCW = SVSwitchMap[7];
        }
        else
        {
            // even # of high switches (2 high switches)
            nearestNullToCW = SVSwitchMap[7];
            nearestNullToCCW = SVSwitchMap[0];
        }

        // assign null states in the state sequence
        _stateSequence[PhyState::Null0] = nearestNullToCW;
        _stateSequence[PhyState::Null6] = nearestNullToCCW;

        // now calculate dead states
        // do this by XOR'ing non-DT states and setting the different bridge to HiZ
        for (int i = 0; i < PhyState::NumStates; i++)
        {
            if (i == PhyState::DT1 || i == PhyState::DT3 || i == PhyState::DT5 || i == PhyState::DT7 || i == PhyState::DT9 || i == PhyState::DT11)
            {
                int prevStateIdx = (i - 1) % PhyState::NumStates;
                int nextStateIdx = (i + 1) % PhyState::NumStates;
                Triple<Inverter::BridgeState> dtState =
                {
                    .A = (_stateSequence[prevStateIdx].A == _stateSequence[nextStateIdx].A) ? _stateSequence[prevStateIdx].A : Inverter::BridgeState::HiZ,
                    .B = (_stateSequence[prevStateIdx].B == _stateSequence[nextStateIdx].B) ? _stateSequence[prevStateIdx].B : Inverter::BridgeState::HiZ,
                    .C = (_stateSequence[prevStateIdx].C == _stateSequence[nextStateIdx].C) ? _stateSequence[prevStateIdx].C : Inverter::BridgeState::HiZ
                };
                _stateSequence[i] = dtState;
            }
        }
    }

    void setVecTarget(Vec2<float> newVecTarget)
    {
        _newVecTarget = newVecTarget;
        _pendingVecTarget = true;
    }
}