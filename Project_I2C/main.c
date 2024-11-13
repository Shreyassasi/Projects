/* PB10 -> I2C2_SCL
	 PB11 -> I2C2_SDA
	 
	 For Sm mode
	 Tlow = Thigh = 10us.
	 TPCLK1 = 62.5 ns
	 CCR = Tlow/tclk1 = 80
*/

#include "stm32f4xx.h"                  // Device header

static void delay(uint32_t ms)
{
	SysTick->CTRL |= (1<<0) | (1<<2) ;
	SysTick->LOAD = (SystemCoreClock/1000)-1;
	
	for(int i=0; i<ms; i++)
	{
		while(!(SysTick->CTRL & (0x10000))) ; /* while this is not set delay is not eleapsed. */
		
	}
	
	SysTick->CTRL &= ~(1<<0) ;
}


static void i2c_generate_start(void) 
{
	I2C1->CR1 |= I2C_CR1_START ;
	while(!(I2C2->SR1 & I2C_SR1_SB)); // polling
}

static void i2c_generate_stop(void) 
{
	I2C1->CR1 |= I2C_CR1_STOP ;
	
}

static void i2c_write(uint8_t data, uint8_t DeviceAddr)
{
	uint32_t status ;
	i2c_generate_start();
	
	I2C2->DR = DeviceAddr;
	while(!(I2C2->SR1 & I2C_SR1_ADDR));
	status = I2C2->SR2;
	
	I2C2->DR = data;
	while(!(I2C2->SR1 & I2C_SR1_BTF));
	
	i2c_generate_stop();
}

static void i2c_read(uint8_t DeviceAddress, uint16_t numbytes)
{
	uint32_t status ;
	uint8_t data;
	
	I2C2->CR1 |= I2C_CR1_ACK;
	
	i2c_generate_start();
	
	I2C2->DR =  DeviceAddress | 0x01; //Read Bit
	while(!(I2C2->SR1 & I2C_SR1_ADDR));
	status = I2C2->SR2;
	
	for(int i = 0; i<numbytes; i++)
	{
		while(!(I2C2->SR1 & I2C_SR1_RXNE));
		data = (uint8_t)I2C1->DR;
		if(i == (numbytes-2))
			I2C2->CR1 &= ~I2C_CR1_ACK;
	}
	 i2c_generate_stop();
}

int main(void)
{
	/* Enable clocks */
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
	
	I2C2->CR1 = 0; 
	I2C2->CR2 |= 16;
	I2C2->CCR |= 80;
	I2C2->TRISE |= 17 ;
	
	I2C2->OAR1 |= (1<<14) ;
	I2C2->CR1 |= I2C_CR1_PE ;
	
	uint8_t data = 0;
	while(1)
	{
	//	i2c_write(data++,0x08<<1);   
		i2c_read(0x08<<1,6);
		delay(400);
	}
	
}
