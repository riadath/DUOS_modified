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


void reboot(){
	SCB->AIRCR = 0x05FA0004;
}



//implement hardfault handler
__attribute__((weak)) void HardFault_Handler(void){
	kprintf("HardFault_Handler Called\n");
	// reboot();
	while(1);
}	



//practice for inline assembly
int do_stuff(int i, int j)
{
  int ret = 0;
  __asm volatile("mov R0, #2");
  __asm volatile("mov R1, #3");
  __asm volatile("add R0, R0, R1");
  __asm volatile(
		"add %[x], %[y], %[z]" 
		: [x] "=r" (ret)
		: [y] "r" (i), [z] "r" (j)
  );
  return ret;
}
//practice for inline assembly

struct t_task_tcb{
	uint32_t magic_number; //here it is 0xFECABAA0
	uint16_t task_id; //a unsigned 16 bit integer starting from 1000
	void *psp; //task stack pointer or stackframe address
	uint16_t status; //task status: running, waiting, ready, killed, or terminated
	uint32_t execution_time; //total execution time (in ms)
	uint32_t waiting_time; //total waiting time (in ms)
	uint32_t digital_sinature; //current value is 0x00000001
} TCB_TypeDef;
typedef struct dev_t
{

	char name[32]; // Device name or symbol
	uint32_t t_ref; //Number of open count
	uint8_t t_access; //open type O_RDONLY, O_WRDONLY, O_APPEND
	uint32_t *op_addr; //Address of the datastructure operations
}dev_table;

__attribute__((weak)) void SVCall_Handler(void) {

	__asm volatile (
		"TST lr, #4\n"
        "ITE EQ\n"
        "MRSEQ r0, MSP\n"
    	"MRSNE r0, PSP\n"
		"MOV r1, r0\n"
		"B SVC_Handler_Main\n"
	);

}

void SVC_Handler_Main( uint32_t *svc_args )
{
	uint32_t svc_number = 0;
	svc_number = ((char *)svc_args[6])[-2] ;
	kprintf("svc number %d\n",svc_number);
}



void kmain(void)
{
	__sys_init();

	unsigned int empty[256];
	for(int i = 0;i < 256;i++){
		empty[i] = 0;
	}
    task_init_env_2(empty+256);

   __asm volatile (
        ".global task_init_env_2\n"
        "task_init_env_2:\n"
	        "msr psp, r0\n"
	        "mov r0, #3\n"
	        "msr control, r0\n"
	        "isb\n"
    
	);

    __asm volatile ("svc 9");
    __asm volatile ("svc 8");
    __asm volatile ("svc 29");

	kprintf("here back main\n");
	while(1);
}

