#include "stm32f1xx_hal.h"

UART_HandleTypeDef huart1;

#define BUFFER_SIZE 100

uint8_t rx_char;
uint8_t buffer[BUFFER_SIZE];
uint8_t rx_index = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    while (1)
    {
        /* Nhận 1 ký tự */
        if (HAL_UART_Receive(&huart1, &rx_char, 1, HAL_MAX_DELAY) == HAL_OK)
        {
            /* Nếu nhận được ký tự ! */
            if (rx_char == '!')
            {
                /* Gửi lại nội dung trong buffer */
                HAL_UART_Transmit(&huart1, buffer, rx_index, HAL_MAX_DELAY);

                /* Xuống dòng */
                uint8_t newline[] = "\r\n";
                HAL_UART_Transmit(&huart1, newline, 2, HAL_MAX_DELAY);

                /* Xóa buffer */
                rx_index = 0;
            }
            else
            {
                /* Lưu ký tự vào buffer */
                if (rx_index < BUFFER_SIZE - 1)
                {
                    buffer[rx_index++] = rx_char;
                }
                else
                {
                    /* Buffer đầy thì reset */
                    rx_index = 0;
                }
            }
        }
    }
}

/*----------------------------------------------------------
 * System Clock
 * HSI = 8 MHz
 *----------------------------------------------------------*/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1);
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while (1);
    }
}

/*----------------------------------------------------------
 * USART1
 * PA9  -> TX
 * PA10 -> RX
 *----------------------------------------------------------*/
static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;

    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        while (1);
    }
}

/*----------------------------------------------------------
 * GPIO USART1
 * PA9  -> TX
 * PA10 -> RX
 *----------------------------------------------------------*/
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA9 - USART1 TX */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA10 - USART1 RX */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*----------------------------------------------------------
 * HAL UART MSP Init
 *----------------------------------------------------------*/
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
    if (uartHandle->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
}

/*----------------------------------------------------------
 * Error Handler
 *----------------------------------------------------------*/
void Error_Handler(void)
{
    while (1)
    {
    }
}
