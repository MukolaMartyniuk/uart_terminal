#include "uart_terminal.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

#define UART_TERMINAL_ASSERT(expression) \
	if (!(expression))                   \
	{                                    \
		return UART_TERMINAL_ERROR;      \
	}

#define HAL_BUFFER_SIZE 100

#if defined STM32F429xx
#define GPIO_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE
#define RX_PIN GPIO_PIN_9
#define TX_PIN GPIO_PIN_10
#define GPIO_ALTERNATE GPIO_AF7_USART1

#define UART_CLK_ENABLE __HAL_RCC_USART1_CLK_ENABLE
#define UART_INSTANCE USART1
	
#elif defined STM32F411xx
#define GPIO_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE
#define RX_PIN GPIO_PIN_2
#define TX_PIN GPIO_PIN_3
#define GPIO_ALTERNATE GPIO_AF7_USART2

#define UART_CLK_ENABLE __HAL_RCC_USART2_CLK_ENABLE
#define UART_INSTANCE USART2

#endif

static UART_HandleTypeDef uart;

static void GPIO_Init()
{
	GPIO_InitTypeDef gpio = {0};

	GPIO_CLK_ENABLE();

	gpio.Pin = RX_PIN | TX_PIN;

	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	gpio.Alternate = GPIO_ALTERNATE;

	HAL_GPIO_Init(GPIOA, &gpio);
}

static HAL_StatusTypeDef UART_Init(uint32_t baudrate)
{
	UART_CLK_ENABLE();

	uart.Instance = UART_INSTANCE;
	uart.Init.BaudRate = baudrate;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.Mode = UART_MODE_TX_RX;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;

	return HAL_UART_Init(&uart);
}

UART_Terminal_Result_t UART_Terminal_Init(uint32_t baudrate)
{
	HAL_StatusTypeDef hal_result;

	GPIO_Init();

	hal_result = UART_Init(baudrate);
	UART_TERMINAL_ASSERT(hal_result == HAL_OK);

	return UART_TERMINAL_OK;
}

UART_Terminal_Result_t UART_Terminal_Print(char *str, uint16_t size)
{
	HAL_StatusTypeDef hal_result;

	hal_result = HAL_UART_Transmit(&uart, (uint8_t *)str, size, HAL_MAX_DELAY);
	UART_TERMINAL_ASSERT(hal_result == HAL_OK);

	return UART_TERMINAL_OK;
}

UART_Terminal_Result_t UART_Terminal_Scan(char *str, uint16_t size)
{
	HAL_StatusTypeDef hal_result;

	hal_result = HAL_UART_Receive(&uart, (uint8_t *)str, size, HAL_MAX_DELAY);
	UART_TERMINAL_ASSERT(hal_result == HAL_OK);

	return UART_TERMINAL_OK;
}

/**
 * Redefine low level output API
 */
#if defined __GNUC__ && !defined __ARMCC_VERSION
int _write(int file, char *ptr, int len)
{
	(void)file;

	UART_Terminal_Print(ptr, len);
	return len;
}
#else
int fputc(int ch, FILE *f)
{
	(void)f;
	char ch_copy = ch;

	UART_Terminal_Result_t result = UART_Terminal_Print(&ch_copy, 1);

	return result == UART_TERMINAL_OK ? ch : EOF;
}
#endif

/**
 * Redefine low level input API
 *
 * TODO: Cover with tests
 */
#if defined __GNUC__ && !defined __ARMCC_VERSION
int _read(int file, char *ptr, int len)
{
	(void)file;

	UART_Terminal_Scan(ptr, len);
	return len;
}
#else
int fgetc(FILE *f)
{
	(void)f;
	char ch_copy;

	UART_Terminal_Result_t result = UART_Terminal_Scan(&ch_copy, 1);

	return result == UART_TERMINAL_OK ? ch_copy : EOF;
}
#endif
