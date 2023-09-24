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
#include <sys.h> // EXTI Config
#include <nvic.h>
#include <test_interrupt.h> // test interrupt function


//includes for svc and pendsvc
#include <syscall.h>
#include <syscall_def.h>

static uint32_t GLOBAL_COUNT_EXTI0 = 0;
static uint32_t GLOBAL_COUNT_EXTI1 = 0;


void reboot(){
	SCB->AIRCR = 0x05FA0004;
}

__attribute__((weak)) void EXTI0_Handler(void){
	//clear pending bit
	if(EXTI->PR & (1<<0)){
		EXTI->PR |= 1<<0;
		reboot();
		// kprintf("EXTI ______0______ interrupt ,%dth time\n",GLOBAL_COUNT_EXTI0);
		// GLOBAL_COUNT_EXTI0 += 1;
	}
}


//implement hardfault handler
__attribute__((weak)) void HardFault_Handler(void){
	kprintf("HardFault_Handler Called\n");
	// reboot();
	while(1);
}	

typedef struct dev_t
{

	char name[32]; // Device name or symbol
	uint32_t t_ref; //Number of open count
	uint8_t t_access; //open type O_RDONLY, O_WRDONLY, O_APPEND
	uint32_t *op_addr; //Address of the datastructure operations
}dev_table;

void kmain(void)
{
	__sys_init();
	dev_table dev[64];
	strcpy(dev[0].name,"usart1");
	dev[0].t_ref = 23;

	kprintf("Device name: %s\n",dev[0].name);
	kprintf("Device t_ref: %d\n",dev[0].t_ref);

	while(1){
		
	}
	
	//trigger svc
	// __asm__ volatile("svc #0");
}

