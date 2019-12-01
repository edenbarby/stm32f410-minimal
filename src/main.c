#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"


void init_sysclk(void);


int main(void){
    LL_GPIO_InitTypeDef gpio_init_struct;

    init_sysclk();

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_StructInit(&gpio_init_struct);
    gpio_init_struct.Mode = LL_GPIO_MODE_OUTPUT;
    gpio_init_struct.Pin  = LL_GPIO_PIN_5;
    LL_GPIO_Init(GPIOA, &gpio_init_struct);

    while(1) {
        LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
        LL_mDelay(500);
    }
    return 0;
}

/* Clock configuration:
** PLL Source:        HSI
** PLLM:              4
** PLLN:              50
** PLLP:              2
** SYSCLK Source:     PLL
** SYSCLK:            100MHz
** AHB1 Prescaler:    1
** AHB1:              100MHz
** HCLK:              100MHz
** APB1 Prescaler:    2
** APB1:              50MHz
** APB2 Prescaler:    1
** APB2:              100MHz
** VDD:               3.3V
** Flash Wait States: 3 (see table 6 from RM0401)
*/
void init_sysclk(void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 50, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1);

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_SetSystemCoreClock(100000000);

    LL_Init1msTick(100000000);
}
