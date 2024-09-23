#ifndef __MAIN_H
#define __MAIN_H

#include "stm32g4xx_hal.h"
#include "IHM16M1_impl.h"

#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

#endif // __MAIN_H