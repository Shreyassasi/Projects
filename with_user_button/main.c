#include<stdint.h>
#include "stm32f407xx.h"

/*
   PD12 - GREEN LED
	 PD13 - ORANGE LED
	 PD14 - RED LED
	 PD15 - BLUE LED
	 
	 PA0 - USER SWITCH
*/


/***
    1. identify the hardware required,connect it.
    2. Power up the peripheral (Clock or Power) i.e we need to enable the clock to GPIO.
         refer AHB1 bus for reference.
         we need to find the base and offset value of register.
         We need to enable bit no 3 in GPIO D.
         Address offset: 0x30 Base Address : 0x4002 3800 - 0x4002 3BFF  // 4002 3830
         (GPIOx_ODR)  Address offset: 0x14  Base Address: 0x40020400
         (GPIOx_MODER)  Address offset: 0x00  Base Address: 0x40020400 // 0x40020400

    3. see the block and identify the revelant registers. i.e go to clock-> RCC registers->(RCC_AHB1ENR)
					GPIOx_ODR
					GPIOx_MODER
					GPIOx_IDR
		
    4. Map the registers into the program - this is already done by CMSIS
    5. Read/Write registers with appropriate values
    6. Write a small application code.

CLK = 1000Hz

1000 cycles -> 1sec
1 cycle -> 1 ms
***/

void SysTick_Handler(void)
{
	
	
	
}
static void delay(uint32_t ms)
{
	SysTick->CTRL |= (1<<0) | (1<<2) ;
	SysTick->LOAD = (SystemCoreClock/1000)-1;
	
	for(int i=0; i<ms; i++)
	{
		while(!(SysTick->CTRL & (0x10000))) ; /* while this is not set delay is not eleapsed. */
		
	}
	
	SysTick->CTRL &= ~(1u<<0) ;
}

int main(void)
{
	RCC->AHB1ENR |= (1<<0) | (1<<3) ;
	
	while ( !(RCC->AHB1ENR & ((1<<0) | (1<<3)) )) ;  /* UNLESS thses clock are configured dont move into the loop. */
	GPIOD->MODER |= (1<<24) | (1<<26) | (1<<28) | (1<<30) ; /*Setting the bit position for led blinking. */
	GPIOD->MODER &= (1<<25) | (1<<27) | (1<<29) | (1u<<31) ; /*clearing the bits,1u cz shifting the MSB(31).Im saying to keep this number as unsigned. */
	
	GPIOD->MODER &= ~ ((1u<<0) | (1<<1)) ;
	
	while(1)
	{
		if (GPIOA->IDR & (1<<0))
		{
			volatile int i = 0;
			GPIOD->ODR |= (1<<12) | (1<<13) | (1<<14) | (1<<15) ;
			for(i=0;i<2000000;i++);
			GPIOD->ODR &= ~ (1u<<12) | (1<<13) | (1<<14) | (1<<15) ;
			for(i=0;i<2000000;i++);
		}
		
		
		
	}
}
