#ifndef __ADC_INTERFACE
#define __ADC_INTERFACE

#include "stm32g4xx_hal.h"

namespace ADC
{
    enum Mode
    {
        Polling =       0,
        Continuous =    1,
        NumModes
    };

    class Conversion
    {
        int value;
        long timestamp;
        bool valid;
    };

    class Interface
    {
        virtual bool init();
        virtual void setMode(Mode mode);
        virtual void setRate(int rate);                                 // set number of automatic samples per second
        virtual void setCallback();                                     // configure a callback automatic sampling completes
        virtual bool poll(int adc, int channel, Conversion* result);    // triggers a blocking conversion and returns the result
        virtual bool pollAll(int adc);                                  // triggers blocking conversions on all configured channels
        virtual bool get(int adc, int channel, Conversion* result);     // gets the latest conversion from a specified channel
    };
}

#endif // __ADC_INTERFACE