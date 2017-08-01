

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

#include "common.h"

uint32_t motorSpeed;
HAL_StatusTypeDef rc;
extern uint32_t isMotorRunning;     //to check motor running
extern uint32_t isADCInitialized;   //ADC initialized ??

void rotateMotor(int mode)
{
    if(mode!=CMD_INTERACTIVE)
        return;

    uint32_t direction;
    fetch_uint32_arg(&direction);
    fetch_uint32_arg(&motorSpeed);
    if(motorSpeed < 0 && motorSpeed > 100)
         return;
    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOA_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    
    //initialize GPIO
    
    GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    //initialize GPIO for timer
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    //timer initialization
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
    isMotorRunning = 1;
}

ADD_CMD("runDc",rotateMotor," <direction 0 for clockwise, 1 for anticlockwise> <Speed> Runs DC motor")

void DCStop(int mode)
{
    TIM8->BDTR=0x00000C00;			   //Loading BDTR Registor 
}
ADD_CMD("StopDC",DCStop,"Stop DC Motor")

int ADCvalue;
int NumRotation;

ADC_HandleTypeDef hadc;
ADC_ChannelConfTypeDef config;

void InitializeADC(int mode)
{
    if(mode != CMD_INTERACTIVE)
    {
        return;
    }
 
    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOB_CLK_ENABLE();
    __ADC34_CLK_ENABLE();
    __HAL_RCC_ADC34_CONFIG(RCC_ADC34PLLCLK_DIV1);

    GPIO_InitStruct.Pin = (GPIO_PIN_0);
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    hadc.Instance = ADC3;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
    hadc.Init.Resolution = ADC_RESOLUTION12b;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc.Init.EOCSelection = EOC_SINGLE_CONV;
    hadc.Init.LowPowerAutoWait = DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.NbrOfConversion = 1;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.NbrOfDiscConversion = 0;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.ExternalTrigConvEdge = 0;
    hadc.Init.DMAContinuousRequests = DISABLE;
    hadc.Init.Overrun = OVR_DATA_OVERWRITTEN;
    HAL_StatusTypeDef rc;
    rc = HAL_ADC_Init(&hadc);
    if(rc != HAL_OK)
    {
        printf("ADC3 initialization failed with rc=%u\n",rc);
    }
    config.Channel = 12;
    config.Rank = 1;  
    config.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
    config.SingleDiff = ADC_SINGLE_ENDED;
    config.OffsetNumber = ADC_OFFSET_NONE;
    config.Offset = 0;


    rc = HAL_ADC_ConfigChannel(&hadc,&config);
    if(rc != HAL_OK) 
    {
        printf("ADC channel configure failed with rc=%u\n",(unsigned)rc);
        return ;
    }
    isADCInitialized = 1;
}
ADD_CMD("ADCinit",InitializeADC,"   ADC initialization")

//read ADC value

void ADCread(void)
{
    HAL_StatusTypeDef rc;
    rc = HAL_ADC_Start(&hadc);
    if(rc != HAL_OK)
    {
        printf("ADC start failed with rc=%u\n",(unsigned)rc);
        return ;
    }


    rc = HAL_ADC_PollForConversion(&hadc, 100);
    if(rc != HAL_OK) 
    {
        printf("ADC poll for conversion failed with rc=%u\n",(unsigned)rc);
        return ;
    }
    ADCvalue = HAL_ADC_GetValue(&hadc); 
    rc = HAL_ADC_Stop(&hadc);
    if(rc != HAL_OK)
    {
        printf("ADC stop failed with rc=%u\n",(unsigned)rc);
    return ;
    }
    uint32_t tempADCvalue = ADCvalue;
    uint32_t NumRotation = tempADCvalue*2.14;  
    printf("RPM is %i\n",(int)NumRotation);
}
//controlling speed
void CloseLoopControl(void)
{   
    ADCread();
    if(ADCvalue >= 3300)
    {
        motorSpeed--;                            
        TIM15->CCR1= motorSpeed;                       
        printf("speed %i\n",(int)motorSpeed);
        printf("Decreasing\n");
    }
    else if(ADCvalue <= 2500)
    {
        motorSpeed++;                            
        TIM15->CCR1=motorSpeed; 
        printf("speed %i\n",(int)motorSpeed);
        printf("Increasing\n");
    }
        
    else if(ADCvalue >= 2500 && ADCvalue <=3300)
    {
        TIM15->CCR1=motorSpeed;  
        printf("speed %i\n",(int)motorSpeed);        
        printf("Normal speed\n");
    }
            
}


