#include "stm32f4xx.h"                  // Device header

static char *str = "Hello\n";
volatile tx_count;

static void init_UART1()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ; /* Enable Port A Clock */
	RCC->APB1ENR |= (1<<17) ; /* Enable USART2 Clock */
	
	/* Configure GPIO for USART2 */
	GPIOA->MODER |= (1<<5) | (1<<7) ;
	GPIOA->MODER &= ~ ((1u<<4) | (1<<6)) ;
	GPIOA->AFR[0] = (0x07 << 8) | (0x07 << 12) ;
	
	USART2->CR1 |= (1<<13) | (1<<3) | (1<<2) ;
	USART2->BRR  = (0x68<<4) | (0x03) ; /* for 9600 baud rate setting this value, 0x68 is hexa value of 104(decimal) this value was derived from baud rate calculation formula */
	
	
}

static void UART_Tx (char data)
	{
		while(!(USART2->SR & USART_SR_TXE)) ; /*This is polling which means that, when is not set that is when we want to be in this loop */
		USART2->DR = data ;
	}

void I2C2_EV_IRQHandler(void)
{
	uint32_t status1, status2;
	uint8_t data;
	
	status1 = I2C2->SR1;
	status2 = I2C2->SR2;
	
	if(status1 & I2C_SR1_ADDR)
		status1 &= ~I2C_SR1_ADDR;
	
	if(status1 & I2C_SR1_TXE)
	{
		I2C2->DR = str[tx_count++];
		if(tx_count > 5)
			tx_count = 0;
		
	}
	
	if(status1 & I2C_SR1_RXNE)
	{
		data = (uint8_t)I2C2->DR;
		UART_Tx(data);
		
	}
}

	
int main(void)
	
{
	init_UART1();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN ; // enabling using macros.
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN ;
	
	/* Configure GPIOs */
	
	GPIOB->MODER &= ~(3u<<20) ;
	GPIOB->MODER |=  (1<<21) ;  //AF
	
	GPIOB->MODER &= ~(3u<<22) ;
	GPIOB->MODER |=  (1<<23) ;  //AF
	
	/* In I2C both SCL and SDA are open drain mode,so need to configure GPO in open drain mode */
	GPIOB->OTYPER |= (1<<10) | (1<<11) ;
	
	/* We can you internal pull-up resistor instead of external pull-up resistor. */
	GPIOB->PUPDR |= (1<<20) | (1<<22) ;
	
	GPIOB->AFR[1] |= (0x04<<4) | (0x04) ;
	I2C2->CR1 = I2C_CR1_SWRST;
	I2C2->CR1 = 0; 
	
	
	I2C2->OAR1 |= (1<<14) ;
	I2C2->OAR1 |=  (0x08<<1);
	
	
	
	
	I2C2->CR1 |= I2C_CR1_PE ;
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C2->CR2 |= I2C_CR2_ITEVTEN;
	NVIC_EnableIRQ(I2C2_EV_IRQn);
	

	
	while(1)
	{
		
	}
	
	
}