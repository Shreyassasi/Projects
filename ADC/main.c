#include "stm32f407xx.h"      // Devide Header

/* 1. Enable the clock for ADC & GPIO --> RCC_APB2ENR < 36 MHz*/
/* 2. Set GPIO pin to Analog Mode */
/* 3. The ADC is powered on by setting the ADON bit in the ADC_CR2 register */
/* 4. Conversion starts when either the SWSTART or the JSWSTART bit is set.*/
/* 5. L[3:0] bits in the ADC_SQR1 register --> Total number of conversion */
/* 6. The regular channels and their order in the conversion sequence must be selected in the ADC_SQRx registers. */

void ADC_IRQHandler(void) ;
volatile uint32_t adc_val; /* globalising the var as shared btw main and interrupt handler. */

/*
why declaring the a variable "volatile" when its global? Like this "volatile uint32_t adc_val; "

we are not calling the function in main so the compiler may assume you are not calling the function, so any changes made to it is not valid.
so we tell compiler this variable may be called or not so dont optimize it.
*/

void ADC_IRQHandler(void) /* Interrupt handler function.  */
{
	
	adc_val = ADC1->DR ;/*Reaading the data registers clears the EOC flag,so pointing towards */
	ADC1->CR2 |= (1<<30); /* triggering a conversion. */
	
}


int main(void)
{
	
	RCC->APB2ENR |= (1<<8) ;
	RCC->AHB1ENR |= (1<<0) ;
	
	
	GPIOA->MODER |= (1<<10) | (1<<11) ;
	
	ADC1->SQR1 &= ~ (0x0Fu << 20);
	ADC1->SQR3 = 0x05;
	
	ADC1->CR1 |= (1<<5); /* Enabling the EOC bit for innterupt.We need to tell NVIC that this might generate interrupt do not mask it. */
	NVIC_EnableIRQ(ADC_IRQn); /*Enabling interrupt handler in NVIC */

	ADC1->CR2 |= (1<<0); /* ADC IS ON now*/
	ADC1->CR2 |= (1<<30); /* triggering a conversion. */
	while(1)
	{
		
		
	}
	
}