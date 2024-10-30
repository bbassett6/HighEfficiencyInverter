#include "impl/HEI_inverter_impl.hpp"

#if PLATFORM_HEI

namespace Inverter
{
    bool init()
    {
        return true;
    }
    
    bool getPhaseCurrents(Vec3<float>* currents)
    {
        // not sure if this is how it will be implemented 
        //STM_ADC::pollAll(1); to get all data in conversion struct but not sure yet if that will be a list cache thing or what
        // currents->A = (STM_ADC::Conversion::value)-1.65/0.75/0.03;
        // currents->B = (STM_ADC::Conversion::value)-1.65/0.75/0.03;
        // currents->C = (STM_ADC::Conversion::value)-1.65/0.75/0.03;
    }
    
    bool getPhaseVoltages(Vec3<float>* voltages)
    {
        // TODO implement observer of some sort that calculates expected current from duty cycle and compares to average of current readings, 
        // need to be concious of the switching cycle and measure on an off and on pulse or multiple points on on and of to averag per switching cycle. 
        // 
        //
        // perform clarke transform to get A B to alpha beta currents
        // recreate phase voltages from DC bus and pwms
        // sliding mode based rotor position sensor uses i alpha and beta and expected phase voltages to estimate position
        // getPhaseCurrents(Vec3<float> currents);
        // Vec2<float> clarkeCurrents;
    }

    // bool clarkeTransform(Vec2<float>* clarkeCurrents)
    // {
    //     clarkeCurrents->A = currents.A;
    //     clarkeCurrents->B = currents.A*.57735 + currents.B*1.1547;
    // }

    // bool getrotorPosition()
    // {
    //     theta = -1*arctan(ea/eb);
    // }
    
    // bool getBusVoltage(float* voltage)
    // {
    //     //STM_ADC::poll(2,2)
    //     currents->A = (STM_ADC::Conversion::value)-1.65/0.75*101;
    // }
}
#endif // PLATFORM_HEI