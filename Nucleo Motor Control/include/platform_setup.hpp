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
#define LED1_PIN                               GPIO_PIN_13
#define LED1_GPIO_PORT                         GPIOB
#define LED1_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_PIN                               GPIO_PIN_11
#define LED2_GPIO_PORT                         GPIOB
#define LED2_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED3_PIN                               GPIO_PIN_11
#define LED3_GPIO_PORT                         GPIOB
#define LED3_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#endif

#endif // __PLATFORM_SETUP