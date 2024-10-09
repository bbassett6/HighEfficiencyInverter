#ifndef __SVM
#define __SVM

#include "common.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "vecmath.hpp"

namespace SVM
{
    // Maps space vectors to nominal switch states
    Triple<bool> SVSwitchMap[8] = 
    {
        [0] = {.A = 0, .B= 0, .C = 0},
        [1] = {.A = 1, .B= 0, .C = 0},
        [2] = {.A = 1, .B= 1, .C = 0},
        [3] = {.A = 0, .B= 1, .C = 0},
        [4] = {.A = 0, .B= 1, .C = 1},
        [5] = {.A = 0, .B= 0, .C = 1},
        [6] = {.A = 1, .B= 0, .C = 1},
        [7] = {.A = 1, .B= 1, .C = 1}
    };

    // Maps sectors to space vector pairs
    Vec2<int> SVSectorMap[6] =
    {
        [0] = {.A = 1, .B = 2},
        [1] = {.A = 2, .B = 3},
        [2] = {.A = 3, .B = 4},
        [3] = {.A = 4, .B = 5},
        [4] = {.A = 5, .B = 6},
        [5] = {.A = 6, .B = 1}
    };

    enum PhyState
    {
        Null0 =     0,  // null vector
        DT1 =       1,  // dead time
        SW2 =       2,  // cw vector (when viewing the sector on a vector diagram, this is the most clockwise vector)
        DT3 =       3,  // dead time
        SW4 =       4,  // ccw vector
        DT5 =       5,  // dead time
        Null6 =     6,  // null vector
        DT7 =       7,  // dead time
        SW8 =       8,  // ccw vector
        DT9 =       9,  // dead time
        SW10 =      10, // cw vector
        DT11 =      11, // dead time
        NumStates
    };

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

    PhyState _state =   PhyState::Null0;                // SVM physical state machine
    PhyState _nextState = _state;
    int _CWVector =     1;                              // the clockwise-most vector for the current sector
    int _CCWVector =    2;                              // the counter-clockwise-most vector for the current sector
    Vec2<float> _vecTarget = {.A=0, .B=0};              // voltage vector the SVM state machine will produce
    bool _pendingVecTarget = false;                     // whether a new vecTarget has been requested
    Vec2<float> _newVecTarget = {.A=0, .B=0};           // the requested vecTarget
    bool _initialized = false;                          // whether _stateDurationsNanos has been calculated

    void advanceStateMachine();                         // advances the state machine
    void updateStateDurations();                        // calculates new state durations based on _vecTarget
    void setVecTarget(Triple<float> newVecTarget);      // set the SVM voltage vector
}

#endif  // __SVM