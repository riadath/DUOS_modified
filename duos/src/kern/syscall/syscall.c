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
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>
#include <errmsg.h>
#include <kunistd.h>
#include <cm4.h>
#include <types.h>
void __sys_start_task(uint32_t task_psp)
{
	//__asm volatile ("POP {LR}");

	// unsigned int * svc_args;
	// __asm volatile ("MOV %0, R1" : "=r" (svc_args));
	// 	uint32_t task_psp = svc_args[3];
		// for(int i = 0; i < 8; i++) {
		// 	kprintf("svcarg[%d] = %x\n\r", i, svc_args[i]);
	//uint32_t task_psp = task->psp;
	//kprintf("%d\n",task_psp);
	__asm volatile ("MOV R0, %0": :"r"(task_psp));
	
	//kprintf("ok\n");
	__asm volatile ("LDMIA R0!,{R4-R11}");
	//kprintf("ok\n");
	__asm volatile ("MSR PSP, R0");
	// __asm volatile ("MRS R1, CONTROL");
	// __asm volatile ("ORRS R1, R1, #0x1"); //unprivileged
	// __asm volatile ("MSR CONTROL, R1");
	__asm volatile ("ISB 0xf":::"memory");
	
	
	int x;
	__asm volatile("mov %0, r0":"=r"(x):);
	//kprintf("%d\n",x);
	__asm volatile ("MOV LR, 0xFFFFFFFD");
	//kprintf("isb\n");
	__asm volatile ("BX LR");
}

//TODO callno
void syscall(unsigned int *args)
{
	unsigned int svc_number;
	svc_number = ((char *) args[6])[-2];
/* The SVC_Handler calls this function to evaluate and execute the actual function */
/* Take care of return value or code */
	//kprintf("%d\n",svc_number);
	switch(svc_number)
	{
		/* Write your code to call actual function (kunistd.h/c or times.h/c and handle the return value(s) */
		case SYS_read: 
			{
				char *toRead;
				//we need to send pointer or pointer
				//so that we can use the first char pointer as our string to write
				__sys_read(0,&toRead);
				//get arguments
				unsigned int string_add = args[10];

				*((char**)string_add) = toRead;
				break;
			}
			
		case SYS_write:
			{
				unsigned int string_add = args[10];
				char *ch = (char*)string_add;
				// __asm volatile("push {lr}");
				// kprintf("%s\n",ch);
				// __asm volatile("pop {lr}");
				__sys_write(STDOUT_FILENO, ch);
				break;
			}
		case SYS_reboot:
			__sys_reboot();
			break;	
		case SYS__exit:
		{	
			TCB_TypeDef* task = args[16];
			task->status = 4;
			break;
		}
		case SYS_getpid:
		{	
			unsigned int pid_add = args[10];
			
			// for(int i=8;i<=30;i++)
			// {
			// 	pid_add = args[i];
			// 	kprintf("Pid i got %d for idx %d\n",pid_add,i);
			// }
			// for(int i=8;i<=24;i++)
			// {
			// 	TCB_TypeDef* stask = args[i];
			// 	kprintf("task id %d for idx %d\n",stask->task_id,i);
			// }
			TCB_TypeDef* task = args[16];
			__sys_getpid((unsigned int *)pid_add,task->task_id);
			break;
		}
		case SYS___time:
		{
			unsigned int time_add = args[10];
			__sys_gettime((unsigned int *)time_add);
		
			break;
		}
		case SYS_yield:
		{	//this just triggers pendsv
			break;	
		}
		case SYS_start_task:
		{
			unsigned int task = args[18];
			//kprintf("%d\n",task);
			__sys_start_task(task);
			//kprintf("Back from start task\n");
			break;
		}		
		/* return error code see error.h and errmsg.h ENOSYS sys_errlist[ENOSYS]*/	
		default: ;
	}

/* Handle SVC return here */
}

