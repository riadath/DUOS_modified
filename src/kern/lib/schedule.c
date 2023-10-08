#include <schedule.h>
ReadyQ_TypeDef queue;
TCB_TypeDef *current_task,*sleep_task;
uint32_t TASK_ID = 1;

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
    // if(current_task->task_id != 4)
    //     print_task_info(current_task);
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

void __attribute__((naked)) PendSV_Handler(void){
	//Clear all pending interrupts
	SCB->ICSR |= (1<<27);

	//save current context
	__asm volatile(
		"mrs r0, psp\n"
		"stmdb r0!, {r4-r11}\n"
		"push {lr}\n"
	);

	__asm volatile("mov %0, r0" 
		: "=r" (current_task->psp)
		:
	);
	//Schedule next task
	__schedule();

	__asm volatile(
		"mov r0, %0" 
		: 
		:"r"(current_task->psp)
	);
	__asm volatile(
		"ldmia r0!,{r4-r11}\n"
		"msr psp, r0\n"
		"pop {lr}\n"
		"bx lr\n"
	);
}


void retarted_dealy(void){
	int x = 100000;
	while(x--)__asm volatile("nop");
}

void print_entire_queue(void){
    kprintf("Printint entire queue ___________\n");
    kprintf("Queue size = %d\n",queue.size);
    kprintf("Queue max = %d\n",queue.max);
    kprintf("Queue st = %d\n",queue.st);
    kprintf("Queue ed = %d\n",queue.ed);

    for (int i = 0;i < queue.size;i++){
        kprintf("Queue q[%d] = %x\n",i,queue.q[i]);
        print_task_info(queue.q[i]);
    }
    kprintf("END QUEUE PRINT ___________\n");
}
void print_task_info(TCB_TypeDef *task){
	kprintf("_____________TASK INFO_____________\n");
	kprintf("Task ID = %d\n",task->task_id);
	kprintf("Task PSP = %x\n",task->psp);
	kprintf("Task status = %d\n",task->status);
	kprintf("Task runnable = %x\n",task->runnable);
	kprintf("\n");
}