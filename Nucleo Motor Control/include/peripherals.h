#ifndef __PERIPHERALS
#define __PERIPHERALS

#include "stm32g4xx_hal.h"
#include "parameters.h"

#if PLATFORM_P_NUCLEO_IHM03
static ADC_HandleTypeDef hadc1;
static ADC_HandleTypeDef hadc2;
static TIM_HandleTypeDef htim1;
#endif  // PLATFORM_P_NUCLEO_IHM03

#if PLATFORM_HEI
// TODO: Define HEI hardware here as we implement it
static ADC_HandleTypeDef hadc1;
static ADC_HandleTypeDef hadc2;
static TIM_HandleTypeDef htim1;
#endif

#endif // __PERIPHERALS