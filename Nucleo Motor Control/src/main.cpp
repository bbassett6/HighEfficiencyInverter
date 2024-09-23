#include "main.h"
#include "parameters.h"

int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();

	return 0;
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}


static void MX_GPIO_Init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
}

static void MX_ADC1_Init(void)
{
	ADC_MultiModeTypeDef multimode = {0};

	hadc1.Instance 						= ADC1;
	hadc1.Init.ClockPrescaler 			= ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution 				= ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	hadc1.Init.GainCompensation 		= 0;
	hadc1.Init.ScanConvMode 			= ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait 		= DISABLE;
	hadc1.Init.ContinuousConvMode 		= DISABLE;
	hadc1.Init.NbrOfConversion 			= 1;
	hadc1.Init.DiscontinuousConvMode 	= DISABLE;
	hadc1.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests 	= DISABLE;
	hadc1.Init.Overrun 					= ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode 		= DISABLE;

	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_ADC2_Init(void)
{
	hadc2.Instance 						= ADC2;
	hadc2.Init.ClockPrescaler 			= ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc2.Init.Resolution 				= ADC_RESOLUTION_12B;
	hadc2.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	hadc2.Init.GainCompensation 		= 0;
	hadc2.Init.ScanConvMode 			= ADC_SCAN_DISABLE;
	hadc2.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;
	hadc2.Init.LowPowerAutoWait 		= DISABLE;
	hadc2.Init.ContinuousConvMode 		= DISABLE;
	hadc2.Init.NbrOfConversion 			= 1;
	hadc2.Init.DiscontinuousConvMode 	= DISABLE;
	hadc2.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
	hadc2.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc2.Init.DMAContinuousRequests 	= DISABLE;
	hadc2.Init.Overrun 					= ADC_OVR_DATA_PRESERVED;
	hadc2.Init.OversamplingMode 		= DISABLE;

	if (HAL_ADC_Init(&hadc2) != HAL_OK)
	{
		Error_Handler();
	}
}


void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}