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
#include <dev_table.h>
//new includes for assignment 2
#include <sys.h> // EXTI Config
#include <nvic.h>
#include <test_interrupt.h> // test interrupt function


//includes for svc and pendsvc
#include <syscall.h>
#include <syscall_def.h>


void reboot(){
	SCB->AIRCR = 0x05FA0004;
}


void SVC_Handler_Main( uint32_t *svc_args )
{
	uint32_t svc_number = 0;
	//pring string in r0
	kprintf("svc_args[0] = %s\n",svc_args[0]);
	for (int i = 1;i < 3;i++){
		kprintf("svc_args[%d] = %d\n",i,svc_args[i]);
	}

	svc_number = ((char *)svc_args[6])[-2] ;
	kprintf("svc number %d\n",svc_number);

	syscall(svc_args);
}

void SVC_Init(void){
	uint32_t psp_stack[1024];
    PSP_Init(psp_stack + 1024);
   __asm volatile (
		".global PSP_Init\n"
		"PSP_Init:\n"
			"msr psp, r0\n"
			"mov r0, #3\n"
			"msr control, r0\n"
			"isb\n"
	);
}

void kmain(void)
{
	__sys_init();
	SVC_Init();
	__init_dev_table();

	

	kprintf("Hello World\n");
	//load args in r0, r1, r2, r3
	char *str = "my_str";
	//load str to r0
	__asm volatile (
		"mov r0, %[x]\n"
		: 
		: [x] "r" (str)
	);
	
	__asm volatile (
		"mov r1, #2\n"
		"mov r2, #3\n"
		"mov r3, #4\n"
	);
	__asm volatile("svc #45");

	kprintf("here back main\n");
	while(1);
}

