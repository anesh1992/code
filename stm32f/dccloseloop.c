/*CODE To Runs DCMotor In Close Loop*/
#include <stdio.h>
#include <stdint.h> 
#include <string.h> 
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

#include "common.h"
uint32_t Speed=3;

/*Initialisation of DC Motor*/ 
void DCMotor_Initialization(int mode)
{
	if(mode!=CMD_INTERACTIVE)
	{
		return;
	}

/*Enable Timer and Set prescaler*/
__TIM15_CLK_ENABLE();
TIM15->PSC=720;                								/*Used to Set prescaler */                     
TIM15->ARR=100;                								/*Used to  set frequency*/ 
TIM15->CCR1=50;                								/*Used for duty cycle */
TIM15->CCMR1=0x00000068;       								/*Used for mode selection */
TIM15->BDTR=0x00008C00;         							/*It Will set MOE bit,OSSI AND OSSR bit*/
TIM15->CCER=0x00000007;        								/*Used to select PWM channel 1*/
TIM15->EGR=0x00000001;        								/*event generation register*/
TIM15->CR1=0x0081;            								/*It Will enable timer*/

/*Declaration of GPIO Defination and Structure*/
GPIO_InitTypeDef GPIO_InitStruct; 
__GPIOA_CLK_ENABLE();
__GPIOC_CLK_ENABLE();

GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
GPIO_InitStruct.Alternate = 0;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/*Declaration of Gpio Pin 14 and 15 For PWM Which Will Control The Speed Of DC Motor*/
GPIO_InitStruct.Pin = (GPIO_PIN_14 | GPIO_PIN_15);
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
GPIO_InitStruct.Alternate = 1;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
ADD_CMD("init",DCMotor_Initialization,      "Initialise DC motor")

/*Function For Run DC Motor in ClockWIse Direction*/ 
void Run_DCMotor_CW(int mode)
{
if(Speed>=0 && Speed<=100)                                                  /*Check The Speed Of Motor Between 0 To 100*/
{
	if(mode!=CMD_INTERACTIVE)
	{
		return;
	}                                  
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,1);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,0);
	TIM15->BDTR=0x00008C00;                                                   /*Set The Bit Of Register BDTR*/
}
		
}
ADD_CMD("cw",Run_DCMotor_CW,      "<Speed>         Runs DC Motor in Anti ClockWise Direction")

/*Function For Run DC Motor in AntiClockWIse Direction*/
void Run_DCMotor_ACW(int mode)
{        					  
/*Check The Speed It's Equal To Checkspeed Then Run DCMotor In Anti-ClockWise Direction*/
if(0<=Speed && Speed<=100)               				/*Check The Speed Of DC Motor Between 0 to 100*/
{
	if(mode!=CMD_INTERACTIVE)
	{
		return;
	}
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,1);
	TIM15->BDTR=0x00008C00;                                           /*Set The Bit Of Register BDTR*/
}
}
ADD_CMD("acw",Run_DCMotor_ACW,"<Speed>         Runs DC Motor in Anti ClockWise Direction")

/*Function For Stop DC Motor */
void DCmotorStop(int mode)
{
	TIM15->BDTR=0x00000C00;                                                     /*Reset The 8th Bit Of Register BDTR*/
}
ADD_CMD("STOP",DCmotorStop,"Stop DC Motor")

int Flag=0;
int Check_Flag()
{
if(Flag==0)
{
	ADC_Initialization(0);
	DCMotor_Initialization(0);
	Flag=1;
}
	return 1;
}
 
void SPeed_Close_Loop()
{
extern int val;
if (Check_Flag()==1)
{
int State=0;
	if(State==0)
	{
		printf("%i\n",(int)val);
		ADC_Read_Tachometer(0);                                           
		printf("speed %i",(int)Speed);
		Run_DCMotor_CW(0);
		if(val>=3300)
		{
			Speed--; 						    
			TIM15->CCR1=Speed; 					     
			printf("speed %i",(int)Speed);
			printf("I Am Decreasing The Speed Of DC Motor");
		}
		else if(val<=2500)
		{
			Speed++;						    
			TIM15->CCR1=Speed; 
			printf("speed %i",(int)Speed);
			printf("I Am Increasing The Speed Of DC Motor");
		}
		
		else if(val>=2500 && val<=3300)
		{
			TIM15->CCR1=Speed; 			
			printf("The Speed Of Motor Is Valid");
		}
		
	}

	if(State==1)
	{
		printf("%i\n",(int)val);
		ADC_Read_Tachometer(0);                                             
		printf("speed %i",(int)Speed);
		Run_DCMotor_ACW(0);
		if(val>=3300)
		{
			Speed--;                                                   					   
			printf("speed %i",(int)Speed); 
			printf("I Am Decreasing The Speed Of DC Motor");
		}
		else if(val<=2500)
		{
			Speed++;
			TIM15->CCR1=Speed; 
			printf("speed %i",(int)Speed);
			printf("I Am Increasing The Speed Of DC Motor");
		}
		
		else if(val>=2500 && val<=3300)
		{
			TIM15->CCR1=Speed; 
			printf("The Speed Of Motor Is Valid");
		}
		
	}
}
else
{
	return ;
}
}

