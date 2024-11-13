#include<stdint.h>
#include<stm32f407xx.h>
/*
    for STM32f407VG discovery board
    LD4 - Green -->  PD12 //when GPIO led is high then only this LED will glow.
    LD3 - Orange --> PD13 
    LD5 - Red -->    PD14
    LD6 - Blue -->   PD15
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
    4. Map the registers into the program 
    5. Read/Write registers with appropriate values
    6. Write a small application code.

***/


//#define AHB1_ENR_REG *(volatile uint32_t *)0x40023830 /*IM Defreencing the pointer variable. */
//#define GPIOD_MODER  *(volatile uint32_t *)0x40020400 
//#define GPIOD_ODR    *(volatile uint32_t *)0x40020414 

/*
struct GPIO 
{
    volatile uint32_t GPIO_D_MODER ;
    volatile uint32_t GPIO_D_OTYPER;
    volatile uint32_t GPIO_D_OSPEEDR;
   // volatile uint32_t GPIO_D_ODR ;
    
};
*/

int main(void)
{
    volatile int i = 0;

   // struct GPIO *G,P;
   // G = (struct GPIO *) 0x4002000;
    // (*&P).GPIO_D_MODER = 0x0100; // we are taking the address and dereferencing the address.
    // (*G).GPIO_D_MODER = 0x120;
    
   // G -> GPIO_D_MODER = 0x01001;
    
    RCC->AHB1ENR |= (1<<3); /* setting the 3rd bit */
    
    while (! ((RCC->AHB1ENR) & (1<<3) ) ); /* wait for the PORT D clock to be enabled.  */
    
    
    
    GPIOD->MODER &= ~(1u<<29); /* output */
    GPIOD->MODER |= (1<<28);
    
    
    while(1)
    {
      GPIOD->ODR = (1<<14);
      
      for(i=0;1<2000000;i++);
      
      GPIOD->ODR &= ~(1u<<14)  ;
      
      for(i=0;1<2000000;i++);
    }
} 


