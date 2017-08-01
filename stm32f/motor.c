#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "common.h"

#define coil1 GPIO_PIN_6
#define coil3 GPIO_PIN_7
#define coil2 GPIO_PIN_8
#define coil4 GPIO_PIN_9

uint32_t Delay;
uint32_t direction;
uint32_t count;

void Cmdstepperinitialize(int mode)
{
	/* Initiaizing GPIO*/
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Pin = (coil1 | coil2 | coil3 |coil4);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = 0;
  	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

  	 __TIM1_CLK_ENABLE();
  	TIM1->PSC = 1440;    /* Set prescaler */
  	TIM1->CR1 = 0x01;    /* Enable timer */
  	TIM1->CNT = 0;       /* Reset counter */

}

ADD_CMD("stepperinit",Cmdstepperinitialize," ")

void CMDStepper(int mode)
{
  	if(mode!=CMD_INTERACTIVE) return;
	fetch_uint32_arg(&direction);
  	fetch_uint32_arg(&count);
  	fetch_uint32_arg(&Delay);
  
  	while (count > 0)
    {
      	if (direction == 1)
	{

	TIM1->CNT = 0;
	while(TIM1->CNT < Delay)
	    {
	      	asm volatile("nop\n");
	    }
 	HAL_GPIO_WritePin(GPIOC,coil1,1);
 	HAL_GPIO_WritePin(GPIOC,coil2,0);
 	HAL_GPIO_WritePin(GPIOC,coil3,0);
	HAL_GPIO_WritePin(GPIOC,coil4,1);
	  
	TIM1->CNT = 0;
	while(TIM1->CNT < Delay)
	    {
	      	asm volatile("nop\n");
	    }
	HAL_GPIO_WritePin(GPIOC,coil1,1);
  	HAL_GPIO_WritePin(GPIOC,coil2,1);
 	HAL_GPIO_WritePin(GPIOC,coil3,0);
 	HAL_GPIO_WritePin(GPIOC,coil4,0);
	  
	TIM1->CNT = 0;
	while(TIM1->CNT < Delay)
	    {
	      	asm volatile("nop\n");
	    }
	HAL_GPIO_WritePin(GPIOC,coil1,0);
  	HAL_GPIO_WritePin(GPIOC,coil2,1);
 	HAL_GPIO_WritePin(GPIOC,coil3,1);
 	HAL_GPIO_WritePin(GPIOC,coil4,0);
	 
    TIM1->CNT = 0;
	while(TIM1->CNT < Delay)
	    {
	      	asm volatile("nop\n");
	    }
	HAL_GPIO_WritePin(GPIOC,coil1,0);
  	HAL_GPIO_WritePin(GPIOC,coil2,0);
 	HAL_GPIO_WritePin(GPIOC,coil3,1);
 	HAL_GPIO_WritePin(GPIOC,coil4,1);
	 
	}
     if (direction == 0)
	{
	TIM1->CNT = 0;
	 while(TIM1->CNT < Delay)
	    {
	      asm volatile("nop\n");
	    }
 	HAL_GPIO_WritePin(GPIOC,coil1,0);
 	HAL_GPIO_WritePin(GPIOC,coil2,0);
 	HAL_GPIO_WritePin(GPIOC,coil3,1);
	HAL_GPIO_WritePin(GPIOC,coil4,1);
	  
	TIM1->CNT = 0;
	while(TIM1->CNT < Delay)
	    {
	      asm volatile("nop\n");
	    }
	HAL_GPIO_WritePin(GPIOC,coil1,0);
  	HAL_GPIO_WritePin(GPIOC,coil2,1);
 	HAL_GPIO_WritePin(GPIOC,coil3,1);
 	HAL_GPIO_WritePin(GPIOC,coil4,0);
	  
	TIM1->CNT = 0;
	while(TIM1->CNT < Delay)
	    {
	      asm volatile("nop\n");
	    }
	HAL_GPIO_WritePin(GPIOC,coil1,1);
  	HAL_GPIO_WritePin(GPIOC,coil2,1);
 	HAL_GPIO_WritePin(GPIOC,coil3,0);
 	HAL_GPIO_WritePin(GPIOC,coil4,0);
	 
    TIM1->CNT = 0;
	  	while(TIM1->CNT < Delay)
	    {
	      	asm volatile("nop\n");
	    }
	HAL_GPIO_WritePin(GPIOC,coil1,1);
  	HAL_GPIO_WritePin(GPIOC,coil2,0);
 	HAL_GPIO_WritePin(GPIOC,coil3,0);
 	HAL_GPIO_WritePin(GPIOC,coil4,1);
	 
	}
      
      	count--;
    }      
  
}
ADD_CMD("runStepperMotor",CMDStepper,"")
