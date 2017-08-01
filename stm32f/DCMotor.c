

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

#include "common.h"



/*Initialisation of DC Motor*/
void rotateMotor(int mode)
{
    if(mode!=CMD_INTERACTIVE)
        return;
    
    uint32_t motorSpeed;
    uint32_t direction;
    fetch_uint32_arg(&direction);
    fetch_uint32_arg(&motorSpeed);
    if(motorSpeed < 0 && motorSpeed > 100)
         return;
    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOA_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    
    
    GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __TIM8_CLK_ENABLE();
    TIM8->PSC=720;         
    TIM8->ARR=100;         
    TIM8->CCR1=motorSpeed;         
    TIM8->CCMR1=0x00000068;        
    TIM8->BDTR=0x00008C00;     
    TIM8->CCER=0x00000005; 
    TIM8->EGR=0x00000001;      
    TIM8->CR1=0x0081;      		

    if(direction == 1)				        
    {
      
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,0);	
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,1);	
    }
    if(direction == 0)                      
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,1);  
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,0);  
    }
}

ADD_CMD("runDc",rotateMotor," <direction 0 for clockwise, 1 for anticlockwise> <Speed> Runs DC motor")

void DCStop(int mode)
{
    TIM8->BDTR=0x00000C00;			   //Loading BDTR Registor 
}
ADD_CMD("StopDC",DCStop,"Stop DC Motor")

