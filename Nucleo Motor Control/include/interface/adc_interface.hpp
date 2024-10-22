#ifndef __ADC_INTERFACE
#define __ADC_INTERFACE

#include "common.hpp"
#include "types.hpp"
#include <functional>

namespace STM_ADC
{
    class Conversion
    {
        int adc;
        int channel;
        int value;
        long timestamp;
        bool valid;
    };

    bool init();
    void setRate(float rate);                               // set number of automatic samples per second
    void setCallback(std::function<void(int, int)> callback);    // configure a callback when automatic sampling cycle completes
}

#endif // __ADC_INTERFACE