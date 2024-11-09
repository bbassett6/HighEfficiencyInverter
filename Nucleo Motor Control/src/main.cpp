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
		|| !STM_ADC::init()
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
	float speed = 0.0f;

	while(1)
	{
		HAL_Delay(1);
		HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);

		speed += 0.00003;
		if (speed > 2 * PI / 10)
			speed = 2 * PI / 10;

		angle += speed;
		// float tempAngle = sinf(angle) * PI / 6 + (5 * PI / 3) + (PI / 6);
		float power = 0.30f;
		SVM::setVecTarget((Vec2<float>){.a = {cosf(angle) * power, sinf(angle) * power}});
	}

	return 0;
}

// Note: when compiling STM32HAL with CPP, weakly linked callbacks like this
// need to be prefixed with `extern "C"`	
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}