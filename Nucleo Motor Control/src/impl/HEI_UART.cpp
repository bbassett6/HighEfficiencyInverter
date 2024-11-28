#include "interface/uart_interface.hpp"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

namespace UART
{
    bool init()
    {
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        // Enable DMA interrupts
        HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

        // Configure DMA 1 channel 4
        hdma_usart1_tx.Instance = DMA1_Channel4;
        hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_tx.Init.Mode = DMA_NORMAL;
        hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
        {
            return false;
        }
        __HAL_LINKDMA(&huart1,hdmatx,hdma_usart1_tx);

        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);

        huart1.Instance                     = USART1;
        huart1.Init.BaudRate                = UARTParameters::baudrate;
        huart1.Init.WordLength              = UART_WORDLENGTH_8B;
        huart1.Init.StopBits                = UART_STOPBITS_1;
        huart1.Init.Parity                  = UART_PARITY_NONE;
        huart1.Init.Mode                    = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl               = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling            = UART_OVERSAMPLING_16;
        huart1.Init.OneBitSampling          = UART_ONE_BIT_SAMPLE_DISABLE;
        huart1.AdvancedInit.AdvFeatureInit  = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart1) != HAL_OK)
        {
            return false;
        }

        // USART1 GPIO Configuration
        // PB6     ------> USART1_TX
        // PB7     ------> USART1_RX
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin         = GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate   = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        return true;
    }

    void transmit(unsigned char* buffer, int size)
    {
        // HAL_UART_Transmit_DMA(&huart1, buffer, size);
        HAL_UART_Transmit(&huart1, buffer, size, 10);
    }

    // TODO: implement later. will be complicated
    void registerReceiveHandler(std::function<void()> handler)
    {

    }
}

extern "C" void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

extern "C" void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}