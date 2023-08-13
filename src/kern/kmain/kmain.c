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


//new includes for assignment 2
#include <sys.h> // ALL NVIC related functions
#include <test_interrupt.h> // test interrupt function


void EXTI0_Init(void){
	RCC->APB2ENR |= 1<<14; //enable SYSCFG clock
	SYSCFG->EXTICR[0] &= ~(0x000F<<0); //select PA0 as source of EXTI0
	EXTI->IMR |= 1<<0; //unmask EXTI0
	EXTI->RTSR |= 1<<0; //select rising edge trigger
	EXTI->FTSR &= ~(1<<0); //disable falling edge trigger
	__NVIC_EnableIRQn(EXTI0_IRQn); //enable EXTI0 interrupt
	__NVIC_SetPriority(EXTI0_IRQn, 2); //set priority to 2
}


__attribute__((weak)) void EXTI0_Handler(void){
	//clear pending bit
	if(EXTI->PR & (1<<0)){
		EXTI->PR |= 1<<0;
		kprintf("EXTI0 interrupt occured\n");
	}


}



void kmain(void)
{
	__sys_init();
	
	
}

