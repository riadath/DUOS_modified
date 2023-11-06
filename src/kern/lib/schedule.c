#include <schedule.h>

ReadyQ_TypeDef queue;

TCB_TypeDef *current_task,*sleep_task;
uint32_t TASK_ID = 1000;

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
	

	tcb->psp = stack_start;
	*(--tcb->psp) = DUMMY_XPSR; //xPSR
	*(--tcb->psp) = (uint32_t)task; //PC
	*(--tcb->psp) = 0xFFFFFFFD; //LR
	
	//store R0 - R3, R12
	for(int i = 0; i < 5; i++) *(--tcb->psp) = 0x00000000;
	*(--tcb->psp) = (uint32_t)tcb;
	//store R4 - R10
	for(int i = 0; i < 7; i++) *(--tcb->psp) = 0x00000000;
}


void start_exec(void){
	if(queue.size == 0)return;
	current_task = pop(); 
	if(current_task->magic_number != 0xFECABAA0
	|| current_task->digital_sinature != 0x00000001){
		kprintf("Invalid task\n");
		return;
	}
	current_task->status = RUNNING;
	start_task(current_task->psp);
}

void __set_sleep(TCB_TypeDef *task){
	sleep_task = task;
	return;
}

// void __attribute__((naked)) PendSV_Handler(void){
// 	//Clear all pending interrupts
// 	uint32_t x;
// 	SCB->ICSR |= (1<<27);

// 	//save current context
// 	__asm volatile(
// 		"mrs r0, psp\n"
// 		"stmdb r0!, {r4-r11}\n"
// 		"push {lr}\n"
// 	);

// 	__asm volatile("mov %0, r0" 
// 		: "=r" (current_task->psp)
// 		:
// 	);
// 	//Schedule next task
// 	__schedule();

// 	__asm volatile(
// 		"mov r0, %0" 
// 		: 
// 		:"r"(current_task->psp)
// 	);
// 	__asm volatile(
// 		"ldmia r0!,{r4-r11}\n"
// 		"msr psp, r0\n"
// 		"pop {lr}\n"
// 		"bx lr\n"
// 	);
// }

//-------------helper functions----------------
#define STOP 		1000000
#define TASK_COUNT 	10



