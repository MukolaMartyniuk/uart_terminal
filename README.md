# UART Terminal

## Description
An application for inputting and outputting information to the console from the microcontroller. Input and output works through the UART data transfer interface. To connect the board through the COM port, you need the Tera Term program, which emulates a terminal window.

## Hardware requirements
+ STM32F429 Discovery

## Software requirements
+ Programming language C
+ Keil uVision 5
+ Tera Term

## Instructions for use
To work with the application, you need to download uart_terminal.uvprojx and open it in Keil uVision 5. You also need to download Tera Term. In Tera Term, connect the board through the COM port. Run the program in Keil. The function `printf("%s", test_str)` is used for output to the console.
