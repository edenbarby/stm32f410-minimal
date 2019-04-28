// #include <stdlib.h>

#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"


int main(void){
    /* Configure the system clock */
    /* Clock init stuff */
    /**
      * @brief  System Clock Configuration
      *         The system Clock is configured as follow :
      *            System Clock source            = PLL (HSE)
      *            SYSCLK(Hz)                     = 100000000
      *            HCLK(Hz)                       = 100000000
      *            AHB Prescaler                  = 1
      *            APB1 Prescaler                 = 2
      *            APB2 Prescaler                 = 1
      *            HSE Frequency(Hz)              = 8000000
      *            PLL_M                          = 8
      *            PLL_N                          = 400
      *            PLL_P                          = 4
      *            VDD(V)                         = 3.3
      *            Main regulator output voltage  = Scale1 mode
      *            Flash Latency(WS)              = 3
      * @param  None
      * @retval None
      */
    /* Enable HSE oscillator */
    LL_RCC_HSE_EnableBypass();
    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1)
    {
    };

    /* Set FLASH latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 400, LL_RCC_PLLP_DIV_4);
    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1)
    {
    };

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    };

    /* Set APB1 & APB2 prescaler */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    /* Set systick to 1ms */
    LL_Init1msTick(100000000);
    //SysTick_Config(100000000 / 1000);

    /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
    SystemCoreClock = 100000000;

    /* Let's pick a pin and toggle it */

    /* Use a structure for this (usually for bulk init), you can also use LL functions */
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable the GPIO clock for GPIOA*/
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    /* Enable clock for SYSCFG */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

    /* Set up pin A5, the LED2 pin. */
    LL_GPIO_StructInit(&GPIO_InitStruct);                   // init the struct with some sensible defaults
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5;                    // GPIO pin 5; on Nucleo there is an LED
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;         // output speed
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;             // set as output
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;   // make it a push pull
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);                  // initialize PORT A

    /* Toggle forever */
    while(1){
        LL_mDelay(500);
        LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
    }

    return 0;
}
