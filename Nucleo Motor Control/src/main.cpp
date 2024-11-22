#include "main.hpp"

int throttleRaw;

void samplingCallback(uint32_t* result, int length)
{
	//int idx = 0;
	// UART::transmit((unsigned char*)result /* + 4*idx*/, sizeof(uint32_t) / sizeof(char) * length);
	throttleRaw = result[7];
}

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
		|| !UART::init()
		|| !Position::init()
	)
	{
		Error_Handler();
	}
	else
	{
		STM_ADC::setCallback(samplingCallback);
		SVM::setVecTarget((Vec2<float>){.a = {0, 0}});
	}

	float angle = 0.0f;

	// run offset learning
	// bool learningSuccess = OffsetLearning::learnOffset();

	while(1)
	{
		// HAL_Delay(1);
		HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);

		Position::getPosition(&angle);
		angle += (PI / 2);

		float power = ((throttleRaw / 4096.0f) - 0.5f) * 2.0f;
		SVM::setVecTarget((Vec2<float>){.a = {cosf(angle) * power, sinf(angle) * power}});
	}

	// failed to learn
	while(1)
	{
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
	}

	return 0;
}

// Note: when compiling STM32HAL with CPP, weakly linked callbacks like this
// need to be prefixed with `extern "C"`	
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}