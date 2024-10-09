#ifndef __SVM
#define __SVM

#include "common.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "vecmath.hpp"
#include "interface/inverter_interface.hpp"

namespace SVM
{
    // Maps space vectors to nominal switch states
    const Triple<Inverter::BridgeState> SVSwitchMap[8] = 
    {
        [0] = {.A = Inverter::BridgeState::Low,     .B= Inverter::BridgeState::Low,     .C = Inverter::BridgeState::Low},
        [1] = {.A = Inverter::BridgeState::High,    .B= Inverter::BridgeState::Low,     .C = Inverter::BridgeState::Low},
        [2] = {.A = Inverter::BridgeState::High,    .B= Inverter::BridgeState::High,    .C = Inverter::BridgeState::Low},
        [3] = {.A = Inverter::BridgeState::Low,     .B= Inverter::BridgeState::High,    .C = Inverter::BridgeState::Low},
        [4] = {.A = Inverter::BridgeState::Low,     .B= Inverter::BridgeState::High,    .C = Inverter::BridgeState::High},
        [5] = {.A = Inverter::BridgeState::Low,     .B= Inverter::BridgeState::Low,     .C = Inverter::BridgeState::High},
        [6] = {.A = Inverter::BridgeState::High,    .B= Inverter::BridgeState::Low,     .C = Inverter::BridgeState::High},
        [7] = {.A = Inverter::BridgeState::High,    .B= Inverter::BridgeState::High,    .C = Inverter::BridgeState::High}
    };

    // Maps sectors to space vector pairs
    const Vec2<int> SVSectorMap[6] =
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

    void init();                                        // bootstraps the state machine
    void advanceStateMachine();                         // advances the state machine
    void updateStateDurations();                        // calculates new state durations based on _vecTarget
    void updateStateSequence();                         // calculates new FET switching sequence
    void setVecTarget(Triple<float> newVecTarget);      // set the SVM voltage vector
}

#endif  // __SVM