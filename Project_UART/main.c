#include "stm32f407xx.h"
#include <stdio.h>

/*
1. USART2 --> PA2 and PA3
2. Enable Clcok for UART-->RCC_APB2ENR - BIt 4
3. Enanble clock for PortA-->RCC_AHB1ENR-->Bit 0
4. Configure GPIOs for alternate Functionality.

5. USART2_SR -> Bit 6 -> Tx Completed
								Bit 4 -> Rx Not Empty ->Data Received
								
6. USART_CR1 -> Bit 13 UE: USART enable
						 -> Bit 3 TE: Transmitter enable
						 -> Bit 2 RE: Receiver enable
						 
*/

void UART_Tx(char data) ;
static void delay(uint32_t ms) ;
void UART_Tx_str(char *str) ;

volatile int a = 0;

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ; /* Enable Port A Clock */
	RCC->APB1ENR |= (1<<17) ; /* Enable USART2 Clock */
	
	/* Configure GPIO for USART2 */
	GPIOA->MODER |= (1<<5) | (1<<7) ;
	GPIOA->MODER &= ~ ((1u<<4) | (1<<6)) ;
	GPIOA->AFR[0] = (0x07 << 8) | (0x07 << 12) ;
	
	USART2->CR1 |= (1<<13) | (1<<3) | (1<<2) ;
	USART2->BRR  = (0x68<<4) | (0x03) ; /* for 9600 baud rate setting this value, 0x68 is hexa value of 104(decimal) this value was derived from baud rate calculation formula */
	char str[100];
	while(1)
	{
		/* stores the string with numeric value in the str char array (string) */
		sprintf(str,"Hello shreyas %d\r\n",a++) ;
		UART_Tx_str("Hello\r\n") ; /* \r ->Return(Carriage Return)  \n ->New Line (line feed) */
		delay(500);
	}		
	
}

void UART_Tx (char data)
	{
		while(!(USART2->SR & USART_SR_TXE)) ; /*This is polling which means that, when is not set that is when we want to be in this loop */
		USART2->DR = data ;
	}
	
	static void delay(uint32_t ms)
{
	SysTick->CTRL |= (1<<0) | (1<<2) ;
	SysTick->LOAD = (SystemCoreClock/1000)-1;
	
	for (uint32_t i=0; i<ms; i++)
	{
		while(!(SysTick->CTRL & (0x10000))) ; /* while this is not set, delay is not eleapsed. */
		
	}
	
	SysTick->CTRL &= ~(1u<<0) ;
}
	
void UART_Tx_str(char *str)
{

	while(*str)
	{
		UART_Tx(*str++);
		
	}
}
