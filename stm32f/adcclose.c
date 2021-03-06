
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

#include "common.h"
int ADCvalue;
int NumRotation;

void RPMmotor(int mode)
{
	if(mode != CMD_INTERACTIVE)
	{
		return;
	}
	ADC_HandleTypeDef hadc;
 
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

ADC_ChannelConfTypeDef config;

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
	NumRotation=ADCvalue*2.14;	
	printf("val is %i\n",(int)NumRotation);
	
rc = HAL_ADC_Stop(&hadc);
	if(rc != HAL_OK)
	{
		printf("ADC stop failed with rc=%u\n",(unsigned)rc);
		return ;
	}
}

ADD_CMD("RPM",RPMmotor,"  read ADC and update RPM of motor")

