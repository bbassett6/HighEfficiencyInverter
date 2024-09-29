#ifndef __ADC_INTERFACE
#define __ADC_INTERFACE

#include "stm32g4xx_hal.h"
#include "types.hpp"
#include <functional>

namespace STM_ADC
{
    enum Mode
    {
        Polling =       0,
        Continuous =    1,
        NumModes
    };

    class Conversion
    {
        int adc;
        int channel;
        int value;
        long timestamp;
        bool valid;
    };

    template <enum Implementation>
    class Interface
    {
        static bool init();
        static void setMode(Mode mode);
        static void setRate(int rate);                                 // set number of automatic samples per second
        static void setCallback(std::function<void(int)> callback);    // configure a callback when automatic sampling completes
        static bool poll(int adc, int channel, Conversion* result);    // triggers a blocking conversion and returns the result
        static bool pollAll(int adc);                                  // triggers blocking conversions on all configured channels
        static bool get(int adc, int channel, Conversion* result);     // gets the latest conversion from a specified channel
    };
}

#endif // __ADC_INTERFACE