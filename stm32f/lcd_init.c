#include "stm32f3xx_hal.h"

#include "stm32f3xx_hal_gpio.h"

#include "stm32f3_discovery.h"

 /*------------------------------------ Locally defined directives --------------------------------*/
 
 #define MAX_BIT_LENGTH_WRITE 8 // this is the number of bits that is needed to be written to the GPIO pins
 
 #define MAX_BIT_LENGTH_READ   8 // this is the number of bits that is needed to be read from the GPIO pin
 
 #define PWM_FULL_CYCLE_TIME 2 // This is the time interval for the PWM
	 
/*these are the flags that can be used by the functions to check the mode of the gpio pin */

uint8_t write_mode_enabled 	= 0; 

uint8_t read_mode_enabled	= 0;

// this function assumes GPIOD was used

int8_t gpio_pin_init ( uint8_t read_mode)
{
	// read_mode is 1 if doing read
	
	// Either read mode or write mode is specified, which has to be, then initialise the configuration specified there
	
	// Enable the GPIOd
	
	__GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef gpio_init_struct; // GPIO Initialisation structure
	
	gpio_init_struct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;// Pin 3 is R/W, Pin 4 is RS, Pin 7 is E
	
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; // Push pull configuration
	
	gpio_init_struct.Pull = GPIO_PULLDOWN; // Pull down configuration
	
	gpio_init_struct.Speed = GPIO_SPEED_LOW; // High speed configuration
	
	HAL_GPIO_Init(GPIOD,&gpio_init_struct); 



	//Configuring the data bus PD8 to PD15
	
	gpio_init_struct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |GPIO_PIN_11 |GPIO_PIN_12 |GPIO_PIN_13 |GPIO_PIN_14 |GPIO_PIN_15 ; //configure these pins
	
	if (read_mode == 1) // then configure the pins as floating Input pins
	{
		read_mode_enabled	= 1;
		
		write_mode_enabled  = 0;
		
		gpio_init_struct.Mode =  GPIO_MODE_INPUT;
		
		gpio_init_struct.Pull = GPIO_NOPULL; // no pullup resistor is included with this
		
	}
	
	else   // then it is write mode
	{
		write_mode_enabled = 1;
		
		read_mode_enabled  = 0;
		
		gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
		
		gpio_init_struct.Pull = GPIO_PULLDOWN;
	}
	
	gpio_init_struct.Speed = GPIO_SPEED_LOW; // High Speed Pin is enabled
	
	
	/* Write all these configurations to the GPIO port D */
	
	HAL_GPIO_Init(GPIOD,&gpio_init_struct); 
	
	return 0; // successfully initialised... 
	
}

