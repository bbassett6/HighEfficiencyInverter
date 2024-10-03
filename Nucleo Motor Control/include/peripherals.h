#ifndef __PERIPHERALS
#define __PERIPHERALS

#include "stm32g4xx_hal.h"
#include "parameters.h"

// TODO: Delete this file if it turns out to be unnecessary

#if PLATFORM_P_NUCLEO_IHM03
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
#endif  // PLATFORM_P_NUCLEO_IHM03

#if PLATFORM_HEI
// TODO: Define HEI hardware here as we implement it
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim1;
#endif

#endif // __PERIPHERALS