#include "inverterInterface.h"
#include "IHM16M1_impl.h"
#include "types.h"

namespace IHM16M1
{
    class Instance : Inverter::Interface
    {
    public:
        void init()
        {
            // Set GPIO modes for gate drive, current/voltage sense
            for (PinDef pin : PinDefs)
            {
                HAL_GPIO_Init(pin.port, &pin.init);
            }

            // Enable Phases
            HAL_GPIO_WritePin(PinDefs[PinNames::En_U].port, PinDefs[PinNames::En_U].init.Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PinDefs[PinNames::En_V].port, PinDefs[PinNames::En_V].init.Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PinDefs[PinNames::En_W].port, PinDefs[PinNames::En_W].init.Pin, GPIO_PIN_SET);

            // Initialize ADCs
            
        }
        
        void setSwitchState(Inverter::SpaceVector vector)
        {
            Triple<bool> stateMap[Inverter::SpaceVector::NumSpaceVectors] = 
            {
                [Inverter::SpaceVector::V0] = {.U = 0, .V = 0, .W = 0},
                [Inverter::SpaceVector::V1] = {.U = 1, .V = 0, .W = 0},
                [Inverter::SpaceVector::V2] = {.U = 1, .V = 1, .W = 0},
                [Inverter::SpaceVector::V3] = {.U = 0, .V = 1, .W = 0},
                [Inverter::SpaceVector::V4] = {.U = 0, .V = 1, .W = 1},
                [Inverter::SpaceVector::V5] = {.U = 0, .V = 0, .W = 1},
                [Inverter::SpaceVector::V6] = {.U = 1, .V = 0, .W = 1},
                [Inverter::SpaceVector::V7] = {.U = 1, .V = 1, .W = 1}
            };
            HAL_GPIO_WritePin(PinDefs[PinNames::In_U].port, PinDefs[PinNames::In_U].init.Pin, (GPIO_PinState)stateMap[vector].U);
            HAL_GPIO_WritePin(PinDefs[PinNames::In_V].port, PinDefs[PinNames::In_V].init.Pin, (GPIO_PinState)stateMap[vector].V);
            HAL_GPIO_WritePin(PinDefs[PinNames::In_W].port, PinDefs[PinNames::In_W].init.Pin, (GPIO_PinState)stateMap[vector].W);
        }
        
        bool getPhaseCurrents(Triple<float>* currents)
        {

        }
        
        bool getPhaseVoltages(Triple<float>* voltages)
        {

        }
        
        bool getBusVoltage(float* voltage)
        {

        }
    };
}