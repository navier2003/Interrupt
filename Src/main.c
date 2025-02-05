/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Navier
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f401xc.h"
#include "clockConfig.h"



void GPIO_Init(void);
void Interrupt_Config(void);
void EXTI0_IRQHandler(void);



int main(void)
{
	sysClock();
	GPIO_Init();
	Interrupt_Config();



    /* Loop forever */
	while(1)
	{
		GPIOC -> BSRR |= (0x1 << 13);
		delay_ms(5000);
		GPIOC -> BSRR |= (0x1 << 13);
	    delay_ms(5000);
	}
}




void GPIO_Init(void)
{
	// Enable the clock in GPIO Port A
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// Enable the clock in GPIO Port A
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	// Enable the system configuration
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Configure the PA0 as input mode
	GPIOA -> MODER &= ~(0x3 << 0);

	// Configure the pin PA0 as internal no pull-up pull-down
	GPIOA -> PUPDR &= ~(0x3 << 0);

	// Configure the pin PC13 as output mode
	GPIOC -> MODER |= (0x1 << 26);
	GPIOC -> MODER &= ~(0x1 << 27);

	// Confgiure the pin PC13 as push-pull
	GPIOC -> OTYPER &= ~(0x1 << 13);

	// Configure as very high speed
	GPIOC -> OSPEEDR |= (0x3 << 26);

	// Confgiure the pin as internal pull-up
	GPIOC -> PUPDR |= (0x1 << 26);
    GPIOC -> PUPDR &= ~(0x1 << 27);

}

void Interrupt_Config(void)
{
	// Select the source input as PA0
	SYSCFG -> EXTICR[0] &= ~(0xF << 0);

	// Set the interrupt as non maskable
	EXTI -> IMR |= (0x1 << 0);

	// Configure the interrupt as falling edge trigger
	EXTI -> FTSR |= (0x1 << 0);

	// Enable the NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
}


// IRQ Handler
void EXTI0_IRQHandler(void)
{
	// if Pendng bit is set then clear it
	if(EXTI -> PR & (0x1 << 0))
	{
		EXTI -> PR |= (0x1 << 0);
	}
}


