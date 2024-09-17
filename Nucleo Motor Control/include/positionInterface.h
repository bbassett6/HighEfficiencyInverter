#ifndef __POSITION_INTERFACE
#define __POSITION_INTERFACE

/*

PositionInterface creates a common interface for rotary position sensors
E.g. hall sensors, encoders, sensorless, etc.

It may abstract more than one hardware type at a type
E.g. hall at low speed, sensorless at high speed

*/

namespace Position
{
    class Interface
    {
        virtual void setOffset(float offset);       // This offset should be specified such that when the interface reports 0 angle, the rotor is electrically aligned with the alpha axis
        virtual bool getPosition(float* position);
        virtual bool getSpeed(float* speed);
    };
}

#endif // __POSITION_INTERFACE