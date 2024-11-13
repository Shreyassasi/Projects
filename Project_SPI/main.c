#include "stm32f4xx.h"      // Device header

#define ASSERT_SS (GPIOA->ODR &= ~(1u<<2))
#define DE_ASSERT_SS (GPIOA->ODR |= (1u<<2))

/*  PA4 -> NSS
		PA5 -> SCK
		PA6 -> MISO
		PA7 -> MOSI */
		
static void delayms(uint32_t ms)
{
	uint32_t i ;
	SysTick->CTRL |= (1<<0) | (1<<2) ;
	SysTick->LOAD = (SystemCoreClock/1000)-1;
	
	for(i=0; i<ms; i++)
	{
		while(!(SysTick->CTRL & (0x10000))) ; /* while this is not set delay is not eleapsed. */
		
	}
	
	SysTick->CTRL &= ~(1u<<0) ;   //disble
	return ;
}

		
static uint32_t SPI_Communicate(uint8_t data)
{
	while(!(SPI1->SR & SPI_SR_TXE));  //polling
	SPI1->DR = data;
	while(!(SPI1->SR & SPI_SR_RXNE));
	return SPI1->DR;
	
}

static void init_UART2()
{
	RCC->AHB1ENR |= (1<<0) ; /* Enable Port A Clock */
	RCC->APB1ENR |= (1<<17) ; /* Enable USART2 Clock */

	/* Configure GPIO for USART2 */
	GPIOA->MODER |= (1<<5) | (1<<7) ;
	GPIOA->MODER &= ~ ((1u<<4) | (1<<6)) ;
	GPIOA->AFR[0] = (0x07 << 8) | (0x07 << 12) ;

	USART2->CR1 |= (1<<13) | (1<<3) | (1<<2) ;
	USART2->BRR  = (0x68<<4) | (0x03) ; /* for 9600 baud rate setting this value, 0x68 is hexa value of 104(decimal) this value was derived from baud rate calculation formula */
	
}

void UART_Tx(char data)
	{
		while(!(USART2->SR & USART_SR_TXE)) ; /*This is polling which means that, when is not set that is when we want to be in this loop */
		USART2->DR = data ;
	}
	
void UART_printf(char *str)
{

	while(*str)
	{
		UART_Tx(*str++);

	}
}

volatile uint8_t data = 0;

void SPI1_IRQHandler(void)  /*Never make interrupt Handler as static */
{
	data=(uint8_t)SPI1->DR;
	SPI1->DR ='A' ;
	UART_Tx(data);
}

int main(void)
{
	 
	 uint8_t rx_data = 0;
	 RCC->AHB1ENR |= (1<<0) ; //Enable GPIOA
									
	 RCC->APB2ENR |= RCC_APB2ENR_SPI1EN ;
	
	 GPIOA->MODER |= (0x02 << 8) | (0x02 << 10) | (0x02 << 12) | (0x02 << 14) | (0x01 << 4) /* PE2 is configured as output.PE2 will be the slave-selevt */;
	
	 GPIOA->AFR[0] |= (0x05 << 16) | (0x05 << 20) | (0x05 << 24) | (0x05 << 28) ;
	
	 init_UART2(); 
	
	// SPI1->CR1 |=  (1 << 8) | (1<<9) ; //gives the control of slave select .
	// SPI1->CR1 |=  (0x03<<3) ; // 3 shifted to 3 in baud rate generation 
	// SPI1->CR1 |=  (1<<2) ;
	  
   	 SPI1->CR1 |=  (1<<6) ;
//	   SPI1->CR2 |=  (1<<6) ;
	
	//	 NVIC_EnableIRQ(SPI1_IRQn) ;
	
/*  we make take any GPIO to connect to SS pin,
 *	as we want to gpio when it goes high/low.
 *  Here we are taking PE2 as GPIO of slave select.
*/	
	
	while(1)
	{ 
		while(!(SPI1->SR & SPI_SR_RXNE));
		data=(uint8_t)SPI1->DR;
	  SPI1->DR ='A' ;
	  UART_Tx(data);
		/*
		ASSERT_SS;
		rx_data=(uint8_t)SPI_Communicate(data++);
		UART_Tx(rx_data) ;
		delayms(1);
		DE_ASSERT_SS;
		delayms(200);
		*/
	}
}
