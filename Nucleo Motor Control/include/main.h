#ifndef __MAIN_H
#define __MAIN_H

#include "stm32g4xx_hal.h"
#include "IHM16M1_impl.h"
#include "peripherals.h"

#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);

#endif // __MAIN_H