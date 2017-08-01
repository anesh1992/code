
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "common.h"

uint32_t dispAttr;
char *character;
char *string;
void lcdBackLight ()
{
    __GPIOB_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct; // GPIO Initialisation structure
    
    gpio_init_struct.Pin = GPIO_PIN_8;
    
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; // Push pull configuration
    
    gpio_init_struct.Pull = GPIO_PULLDOWN; // Pull down configuration
    
    gpio_init_struct.Speed = GPIO_SPEED_LOW; // High speed configuration
    
    HAL_GPIO_Init(GPIOB,&gpio_init_struct); 
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);
}

void lcd_init()
{
     
    fetch_uint32_arg(&dispAttr);
    lcdBackLight ();
    gpio_pin_init (0);
    GPIOD -> ODR = 0x00000000;
    HAL_Delay(45);
    GPIOD -> ODR = 0x00000120;
    HAL_Delay(10); 
    switch (dispAttr)
    {
        case 0:
            // display off
            GPIOD -> ODR = 0x00000820;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
        case 1:
            //display on, cursor off
            GPIOD -> ODR = 0x00000C20;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
        case 2:
            //display on, cursor on
            GPIOD -> ODR = 0x00000E20;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
        case 3:
            //display on, cursor on flashing
            GPIOD -> ODR = 0x00000F20;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
    }
}
ADD_CMD("initLcd",lcd_init,"0 display off, 1 display on, cuGPIO_PIN_4or off, 2 display on, cuGPIO_PIN_4or on, 3 display on, cuGPIO_PIN_4or on flashing")

void lcd_clrscr(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
    gpio_pin_init (0);
    GPIOD -> ODR = 0x00000120;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
}
ADD_CMD("clrscr",lcd_clrscr,"clear screen")

void lcd_home(int mode)
{
    gpio_pin_init (0);
    GPIOD -> ODR = 0x00000220;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
}
ADD_CMD("home",lcd_home,"lcd home")

void lcd_putc(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
    fetch_string_arg(&character);
    gpio_pin_init (0);
    //function set
    GPIOD -> ODR = 0x00003820;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    //Entry mode set
    GPIOD -> ODR = 0x00000620;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    //Display character  
    GPIOD -> ODR = 0x00000030 | (uint32_t)*character << 8;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    printf("%s\n",character);
}
ADD_CMD("putc",lcd_putc,"single word")

void lcd_puts(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
    fetch_string_arg(&string);
    uint8_t characterCount = 1;
    while(*string != 0)
    {
        if (characterCount > 16)
        {
            gpio_pin_init (0);
            GPIOD -> ODR = 0x0000C020;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        }
        gpio_pin_init (0);
        //function set
        GPIOD -> ODR = 0x00003820;
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        //Entry mode set
        GPIOD -> ODR = 0x00000620;
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        GPIOD -> ODR = 0x00000030 | (uint32_t)*string << 8;
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        string = string + 0x00000001;
        characterCount ++;
    }
}
ADD_CMD("puts",lcd_puts,"string")

void customCharacter(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
    gpio_pin_init (0);
    //function set
    GPIOD -> ODR = 0x00003820;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    //Entry mode set
    GPIOD -> ODR = 0x00000620;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    
    GPIOD -> ODR = 0x00004020;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00000E30;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00004120;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00000030;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00004220;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00001130;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00004320;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00001130;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00004420;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00001130;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00004520;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00001130;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00004620;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00000030;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00004720;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00000E30;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00008020;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);

    GPIOD -> ODR = 0x00000030;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
}

ADD_CMD("customChar",customCharacter,"Custom character")
uint8_t USARTRxBuffer[80];
void receiveUart(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
     /* Configure the GPIO pins for the UART */
     __GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 7;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
     

    /* UART handle declaration */
    UART_HandleTypeDef UartHandle;
    __USART1_CLK_ENABLE();
     
    UartHandle.Instance = USART1;
    UartHandle.Init.BaudRate = UARTBAUDRATE;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
     
    HAL_UART_Init(&UartHandle);
    HAL_UART_Receive(&UartHandle, USARTRxBuffer, 1, 1000);    
} 





