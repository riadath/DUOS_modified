/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, W OTHERWISE) ARISIHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE ORNG IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.

 */
 
#include <sys_init.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <usart.h>
#include <gpio.h>

#include <stm32_peps.h>


//__NVIC_ functions, will be movied to separte file later
/*____________START____________*/


void __NVIC_EnableIRQn(enum IRQn_TypeDef IRQn){
	
}
void __NVIC_DisableIRQn(enum IRQn_TypeDef IRQn){

}

void __NVIC_GetPriority(enum IRQn_TypeDef IRQn){

}
void __NVIC_SetPriority(enum IRQn_TypeDef IRQn,uint32_t priority){

}

void __set_BASEPRI(uint32_t value){

}

//mask all interrupts other than HardFault,NMI and reset
void __disable_irq(){

}
//unmask(enable) all interrupt
void __enable_irq(){

}



/*_____________END____________*/




void kmain(void)
{
	uint8_t digit;
	__sys_init();
	
	GPIO_InitTypeDef gpio_init;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pin |= GPIO_PIN_5;
	gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init(GPIOA,&gpio_init);

	while(1){
		GPIO_WritePin(GPIOA,5,GPIO_PIN_SET);
		__delay_ms(100);
		GPIO_WritePin(GPIOA,5,GPIO_PIN_RESET);
		__delay_ms(200);

		// kprintf(0,"%d",digit);
	}
}

