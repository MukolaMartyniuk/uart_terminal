#include "stm32f4xx_hal.h"
#include "uart_terminal.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

#define ASSERT(expression) \
	if (!(expression))     \
	{                      \
		while (1)          \
			;              \
	}

#define STANDARD_BAUDRATE 115200
#define PRINT_DELAY_MS 2000
#define TEST_STR "Hello world\r\n"

void SysTick_Handler(void)
{
	HAL_IncTick();
}

int main(void)
{
	int result;
	char *test_str = TEST_STR;

	result = HAL_Init();
	ASSERT(result == HAL_OK);

	result = UART_Terminal_Init(STANDARD_BAUDRATE);
	ASSERT(result == UART_TERMINAL_OK);

	while (1)
	{
		// UART_Terminal_Print(test_str, strlen(test_str));
		printf("%s", test_str);
		HAL_Delay(PRINT_DELAY_MS);
	}
}
