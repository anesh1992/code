#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

#include "common.h"

void inputCapture(int mode)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __TIM8_CLK_ENABLE();
    
    TIM_HandleTypeDef hTIM8;

    hTIM8.Instance = TIM8;

    hTIM8.Init.Prescaler = TIM_ICPSC_DIV1;
    hTIM8.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTIM8.Init.Period = 50;
    hTIM8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    hTIM8.Init.RepetitionCounter = 1000;

    HAL_TIM_Base_Init(&hTIM8);

    hTIM8.Channel = 2;

    TIM_IC_InitTypeDef hIC;
    hIC.ICPolarity = TIM_ICPOLARITY_RISING;
    hIC.ICSelection = TIM_ICSELECTION_DIRECTTI; 
    hIC.ICPrescaler = TIM_ICPSC_DIV1;
    hIC.ICFilter = 0x3;

  HAL_TIM_IC_Init(&hTIM8); 

  HAL_TIM_IC_ConfigChannel(&hTIM8, &hIC, 2); 

  HAL_TIM_IC_Start(&hTIM8, 2);
  while(1)
  {
    uint32_t ICvalue = HAL_TIM_ReadCapturedValue(&hTIM8, 2);

    printf("Input capture value:%d\n",(int)ICvalue);
  }

  HAL_TIM_IC_Stop(&hTIM8, 2);
}

ADD_CMD("capture",inputCapture,"    capture")







