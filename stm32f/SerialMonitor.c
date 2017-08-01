/*
	Serial Port Example Code
*/

#include <stdio.h>
#include <string.h>
#include"stm32f3_discovery.h"
#include"stm32f3xx_hal.h"
#include"common.h"

/* UART Handle Declaration */
UART_HandleTypeDef UartHandle;

/* INtialize Serial Port*/
void CmdSerialInit(int mode)
{
 if(mode != CMD_INTERACTIVE) return;
 
 GPIO_InitTypeDef GPIO_InitStruct;

/*Configure the GPIO pins for the UART*/
 __GPIOC_CLK_ENABLE();
 GPIO_InitStruct.Pin       = GPIO_PIN_5;
 GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull      = GPIO_PULLUP;
 GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
 GPIO_InitStruct.Alternate = 7;
 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

 GPIO_InitStruct.Pin       = GPIO_PIN_4;
 GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull      = GPIO_PULLUP;
 GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
 GPIO_InitStruct.Alternate = 7;
 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

 __USART1_CLK_ENABLE();

 /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
 /* UART configured as follows:
 - Word Length = 8 Bits
 - Stop Bit    = One Stop bit
 - Parity      = ODD parity
 - BaudRate    = UARTBAUDRATE baud
 - Hardware flow control disabled (RTS and CTS signals)
 */
 UartHandle.Instance        = USART1;
 UartHandle.Init.BaudRate   = UARTBAUDRATE;
 UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
 UartHandle.Init.StopBits   = UART_STOPBITS_1;
 UartHandle.Init.Parity     = UART_PARITY_NONE;
 UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
 UartHandle.Init.Mode       = UART_MODE_TX_RX;

 if (HAL_UART_Init(&UartHandle) != HAL_OK)
   {
	/* Initialization Error */
	Error_Handler();
   }
}
ADD_CMD("Serialinit",CmdSerialInit,"		Intialize Serial Port")

uint8_t USARTTxBuffer[80];

/* Send Character to the Serial Port */
void CmdSerialSend(int mode)
{
 char *string;
 if(mode != CMD_INTERACTIVE) return;
 
 string = NULL;
 fetch_string_arg(&string);
 if(string != NULL)
 {
  strncpy ((char *)USARTTxBuffer,string,80);
 }
 HAL_UART_Transmit(&UartHandle, USARTTxBuffer,strlen((char *)USARTTxBuffer), 1000);
}
ADD_CMD("Serialsend",CmdSerialSend,"		Send a String to the Serial Port")
