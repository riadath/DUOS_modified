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
#include <unistd.h>

//includes for svc and pendsvc
#include <syscall.h>
#include <syscall_def.h>



void SVC_Handler_Main( uint32_t *svc_args )
{
	syscall(svc_args);
}

void print_device_list(){
	kprintf("\n\n______________________\n\n");
	for (int i = 0;i < device_count;i++){
		kprintf("device name = %s\n",device_list[i].name);
		kprintf("device t_ref = %d\n",device_list[i].t_ref);
		kprintf("device t_access = %d\n",device_list[i].t_access);
		kprintf("device op_addr = %x\n",device_list[i].op_addr);
		kprintf("\n");
	}
}

void __move_to_user(void){
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

void unpriv (void) {
	__asm volatile ("MRS R0, CONTROL");
	__asm volatile ("ORRS R0, R0, #1");
	__asm volatile ("MSR CONTROL, R0");
}

void SVC_Tester(void){
	// test scanf
	char *data = "temp a ja e thakuk";
	scanf(0,&data,5);
	kprintf("data(main) = %s\n",data);

	
	//test fopen and fclose
	char *device_name = "GPIOA";
	uint8_t t_access = 0;
	uint32_t* op_addr = (uint32_t *)GPIOA;
	fopen(device_name,t_access,op_addr);
	print_device_list();

	fclose(op_addr);
	print_device_list();

	//test reboot
	kprintf("Do you want to reboot the OS? (y : 1 /n : 0)\n");
	int if_reboot;
	kscanf("%d",&if_reboot);
	if (if_reboot == 1){
		reboot();
	}
}



//-------------global----------------
TCB_TypeDef __task[MAX_TASKS],__sleep;


void task_1(void){
	kprintf("_____________Task 1_____________\n");
	// while(1);
}
void sleep_state(void){
	kprintf("Sleeping\n");
	while(1);
}



void init_queue(void){
	queue.size = 0;
	queue.max = MAX_TASKS;
	queue.st = 0;
	queue.ed = -1;
}

void queue_add(TCB_TypeDef *task){
	if (queue.size == queue.max){
		return;
	}
	queue.ed = (queue.ed + 1) % queue.max;
	queue.q[queue.ed] = task;
	queue.size++;
}
TCB_TypeDef* pop(){
	if (queue.size == 0){
		queue_add(sleep_task);
	}

	TCB_TypeDef *task = queue.q[queue.st];
	queue.st = (queue.st + 1) % queue.max;
	queue.size--;
	return task;
}



//-------------scheduling functions----------------

void __schedule(void){
    if(current_task->status == RUNNING){
        current_task->status = READY;
        queue_add(current_task);
    }

    TCB_TypeDef *front = pop();
	current_task = front;
    current_task->status = RUNNING;
    return;
}
void __create_task(TCB_TypeDef *tcb, void(*task)(void), uint32_t *stack_start){
	tcb->magic_number = 0xFECABAA0;
	tcb->task_id = TASK_ID++;
	tcb->status = READY;
	tcb->execution_time = 0;
	tcb->waiting_time = 0;
	tcb->digital_sinature = 0x00000001;
	tcb->runnable = task;

	tcb->psp = stack_start;
	*(--tcb->psp) = DUMMY_XPSR; //xPSR
	*(--tcb->psp) = (uint32_t)task; //PC
	*(--tcb->psp) = 0xFFFFFFFD; //LR
	
	//store R0 - R3, R12
	for(int i = 0; i < 5; i++) *(--tcb->psp) = 0x00000000;
	*(--tcb->psp) = (uint32_t)tcb;
	//store R4 - R11
	for(int i = 0; i < 7; i++) *(--tcb->psp) = 0x00000000;
}

void retarted_dealy(void){
	int x = 13423423;
	while(x--)__asm volatile("nop");
}

void __start_task(void){

	if(queue.size == 0){
		return;
	}

	current_task = pop(); //current task = front of queue

	// print_task_info(current_task);
	retarted_dealy();
	
	if(current_task->magic_number != 0xFECABAA0
		|| current_task->digital_sinature != 0x00000001){
		kprintf("Invalid task\n");
		return;
	}
	current_task->status = RUNNING;


	__asm volatile ("MOV R0, %0"
		:
		:"r" (current_task->psp)
	);
	__asm volatile ("LDMIA R0!,{R4-R11}");
	__asm volatile ("MSR PSP, R0");
	__asm volatile ("ISB 0xf" ::: "memory");
	__asm volatile ("MOV LR, 0xFFFFFFFD");
	__asm volatile ("BX LR");
}

void __set_sleep(TCB_TypeDef *task){
	sleep_task = task;
	return;
}

void print_task_info(TCB_TypeDef *task){
	kprintf("_____________TASK INFO_____________\n");
	// kprintf("Task magic number = %x\n",task->magic_number);
	// kprintf("Task ID = %d\n",task->task_id);
	// kprintf("Task execution time = %d\n",task->execution_time);
	// kprintf("Task waiting time = %d\n",task->waiting_time);
	// kprintf("Task digital sinature = %x\n",task->digital_sinature);
	kprintf("Task PSP = %x\n",task->psp);
	kprintf("Task status = %d\n",task->status);
	kprintf("Task runnable = %x\n",task->runnable);
	kprintf("\n");
}


void kmain(void){
	__sys_init();
	__init_dev_table();
	__NVIC_SetPriority(SVCall_IRQn, 1);
	__NVIC_SetPriority(SysTick_IRQn, 0x2);
	__NVIC_SetPriority(PendSV_IRQn, 0xFF); 
	__move_to_user();
	test();
	while(1);
	init_queue();
	__create_task(__task,task_1,(uint32_t*)TASK_STACK_START );
	queue_add(__task );
	__create_task(__task + 1,task_1,(uint32_t*)TASK_STACK_START - TASK_STACK_SIZE);
	queue_add(__task + 1);
	__create_task(&__sleep,sleep_state,(uint32_t*)TASK_STACK_START - 2 * TASK_STACK_SIZE);
	__set_sleep(&__sleep);


	__set_pending(1);
	__start_task();
	
	kprintf("___________END MAIN___________\n");
	while(1);
}

