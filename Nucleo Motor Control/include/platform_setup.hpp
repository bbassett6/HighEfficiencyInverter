#ifndef __PLATFORM_SETUP
#define __PLATFORM_SETUP

#include "common.hpp"

void SystemClock_Config(void);
void GPIO_Init(void);
void Error_Handler(void);

#if PLATFORM_P_NUCLEO_IHM03
#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#endif

#if PLATFORM_HEI
// TODO: change these pin defs to the correct values
#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#endif

#endif // __PLATFORM_SETUP