#include "main.hpp"

int main()
{
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();

	// Set up LED
	GPIO_InitTypeDef ledPinInit = {.Pin = LED_PIN, .Mode = GPIO_MODE_OUTPUT_PP};
	HAL_GPIO_Init(LED_GPIO_PORT, &ledPinInit);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET);

	// Set up peripherals
	if
	(
		   !STM_TIMER::init()
		// || !STM_ADC::init()
		// || !Inverter::init()
		   || !SymmetricPWM::init()
	)
	{
		Error_Handler();
	}
	else
	{
		SVM::setVecTarget((Vec2<float>){.a = {0, 0}});
	}
	float angle = 0.0f;

	while(1)
	{
		HAL_Delay(5);
		HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
		angle += 2 * PI / 1000;
		SVM::setVecTarget((Vec2<float>){.a = {cosf(angle) * 0.1, sinf(angle) * 0.1}});
	}

	return 0;
}

// Note: when compiling STM32HAL with CPP, weakly linked callbacks like this
// need to be prefixed with `extern "C"`	
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}