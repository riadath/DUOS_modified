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
#include <sys.h>



void test_interrupt(void){

}

void blinky_init(){
	GPIO_InitTypeDef gpio_init;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pin |= GPIO_PIN_5;
	gpio_init.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio_init.Pull = 0;
	GPIO_Init(GPIOA,&gpio_init);
}

void on_off(uint32_t delay_ms){
	GPIO_WritePin(GPIOA,5,GPIO_PIN_SET);
	__delay_ms(delay_ms);
	GPIO_WritePin(GPIOA,5,GPIO_PIN_RESET);
	__delay_ms(delay_ms);
}
void kmain(void)
{
	__sys_init();
	
	// __NVIC_DisableIRQn(SysTick_IRQn);
	// __disable_fault_irq();
	while(1){
		kprintf(0,"kaj kortese?\n");
		__delay_ms(1000);
	}
}

