#pragma once

/**
 * Provides API for
 * 1. transmitting text messages between STM32F4 board and PC using UART
 * 2. redirecting standard IO to use this API
 *
 * Supported platforms:
 * STM32F429-DISCO
 * STM32F411-DISCO
 *
 * User can select baudrate passing it to UART_Terminal_Init
 *
 * Prerequisites:
 * 1. SysTick_Handler should be redefined to call HAL_IncTick
 * 2. HAL_Init should be called before UART_Terminal_Init
 */

#include "stdint.h"

/**
 * Return error codes for current API
 */
typedef enum
{
	UART_TERMINAL_OK = 0,
	UART_TERMINAL_ERROR = 1
} UART_Terminal_Result_t;

/**
 * \brief Initialization function for redirecting standard IO to
 * allow sending messages to serial terminal over UART.
 *
 * \param baudrate
 *
 * \returns UART_TERMINAL_OK if successfully initialized or else UART_TERMINAL_INIT_ERROR
 */
UART_Terminal_Result_t UART_Terminal_Init(uint32_t baudrate);

/**
 * \brief Print string to serial terminal over UART
 * Recommended to call stdio functions in user code instead (e.g. printf)
 *
 * \param str string to be sent to PC
 * \param size size of str [strlen(str) can be passed if str is '\0' terminated]
 */
UART_Terminal_Result_t UART_Terminal_Print(char *str, uint16_t size);

/**
 * \brief Read string from serial terminal over UART
 * Recommended to call stdio functions in user code instead (e.g. scanf)
 *
 * \param str pointer to string to be received  from PC
 * \param size size of input buffer
 */
UART_Terminal_Result_t UART_Terminal_Scan(char *str, uint16_t size);

