/*
 * ******************************************************************************
 * @file           : main.c
 * @author         : Navier
 * @brief          : Main program body
 * Date : Jan 29,2025
 ******************************************************************************
 *
 *
 * This file includes the clock configuration setup for STM32F4 MCU.
 * HSE Clock Clock Frequency = 25MHz
 * SYSCLK Frequency = 84MHz
 * APB1 Peripheral Clock = 42MHz
 * APB1 Timer Clock = 84MHz
 * APB2 Peripheral Clock = 84MHz
 * APB2 Timer Clock = 84MHz
 *
 *
 * */


#include "stm32f401xc.h"
#include "clockConfig.h"



void sysClock(void)
{
   // Enable the HSE clock and wait for it to become ready
   RCC -> CR |= RCC_CR_HSEON;
   while(!(RCC -> CR & RCC_CR_HSERDY));

   // Settings related to Flash access control and prefetch buffer
   FLASH -> ACR |= (FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_ICEN);
   FLASH -> ACR |= FLASH_ACR_LATENCY_2WS;

   // Enable the power interface clock and VOS
   RCC -> APB1ENR |= (RCC_APB1ENR_PWREN);
   PWR -> CR |= PWR_CR_VOS_1;

   // Set the AHB1 , APB1 and APB2 prescalers
   RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;
   RCC -> CFGR |= RCC_CFGR_PPRE1_DIV2;
   RCC -> CFGR |= RCC_CFGR_PPRE2_DIV1;

   // Configuring PLL
   RCC -> PLLCFGR |= ((1 << 0) | (1 << 3) | (1 << 4));
   RCC -> PLLCFGR &= ~((1 << 1) | (1 << 2) | (1 << 5));
   RCC -> PLLCFGR |= ((1 << 10) | (1 << 12) | (1 << 14));
   RCC -> PLLCFGR &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 11) | (1 << 13));
   RCC -> PLLCFGR &= ~((1 << 16)|(1 << 17));

   RCC -> CFGR = RCC_PLLCFGR_PLLSRC_HSE;


   // Enable the PLL and wait for it to become ready
   RCC -> CR |= RCC_CR_PLLON;
   while(!(RCC-> CR & RCC_CR_PLLRDY));

   RCC -> CFGR |= RCC_CFGR_SW_PLL;
   while((RCC -> CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_PLL));

   // delay function
   RCC -> APB1ENR |= RCC_APB1ENR_TIM5EN;
   TIM5 -> PSC = 84-1;
   TIM5 -> ARR = 0xFFFF;
   TIM5 -> CR1 |= (1 << 0);
   while(!(TIM5 -> SR & (1 << 0)));
}



void delay_us(uint16_t us)
{
	TIM5 -> CNT = 0;
	while(TIM5 -> CNT < us);
}


void delay_ms(uint16_t ms)
{
	for(int i =0 ; i < ms ; i++)
	{
		delay_us(1000);
	}
}
