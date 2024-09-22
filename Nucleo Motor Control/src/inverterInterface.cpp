#include "inverterInterface.h"
#include "types.h"
namespace Inverter
{
    // ADC.CH0 = 0.04;
    // ADC.CH1 = 0.05;

    class Interface
    {
        bool getBusVoltages(float voltages, ADC* ADC) 
        {
            voltages = (ADC->CH3-1.65)/0.75*101;
            // HV = (voutp-m)*101
            // adc gives (voutp-m)0.075+1.65
        }

        bool getPhaseCurrents(Float3* currents, ADC* ADC)
        {
            currents->A = (ADC->CH0-1.65)/0.75/0.03;
            currents->B = (ADC->CH1-1.65)/0.75/0.03;
            currents->C = (ADC->CH2-1.65)/0.75/0.03;
            // 30mV/A hall effect
            // 0.75V/V diff amp gain + 1.65 dc offset on output
            
        }

    }
}