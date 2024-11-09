#ifndef __UART_INTERFACE
#define __UART_INTERFACE

#include "common.hpp"
#include "types.hpp"
#include <functional>

/*

UART Interface is just a wrapper around the system's uart port

*/

namespace UART
{
    bool init();
    void transmit(unsigned char* buffer, int size);
    void registerReceiveHandler(std::function<void()> handler);
}

#endif // __UART_INTERFACE