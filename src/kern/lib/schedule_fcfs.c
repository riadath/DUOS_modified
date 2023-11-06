#include <schedule_fcfs.h>

//sceduling for first come first serve

ReadyQ_TypeDef queue_fcfs;
TCB_TypeDef *current_task_fcfs,*sleep_task_fcfs;
uint32_t TASK_ID_FCFS = 1000;

void init_queue_fcfs(void){
    queue_fcfs.size = 0;
    queue_fcfs.max = MAX_TASKS;
    queue_fcfs.st = 0;
    queue_fcfs.ed = -1;
}

void queue_add_fcfs(TCB_TypeDef *task){
	if (queue_fcfs.size == queue_fcfs.max){
		return;
	}
	queue_fcfs.ed = (queue_fcfs.ed + 1) % queue_fcfs.max;
	queue_fcfs.q[queue_fcfs.ed] = task;
	queue_fcfs.size++;
}

TCB_TypeDef* pop_fcfs(){
	if (queue_fcfs.size == 0){
		queue_add_fcfs(sleep_task_fcfs);
	}

	TCB_TypeDef *task = queue_fcfs.q[queue_fcfs.st];
	queue_fcfs.st = (queue_fcfs.st + 1) % queue_fcfs.max;
	queue_fcfs.size--;
	return task;
}

void __schedule_fcfs(void){
    // first come first serve
    current_task_fcfs = pop();
    current_task_fcfs->status = RUNNING;
    return;
}

void start_exec_fcfs(void){
	if(queue_fcfs.size == 0)return;
	current_task_fcfs = pop(); 
	if(current_task_fcfs->magic_number != 0xFECABAA0
	|| current_task_fcfs->digital_sinature != 0x00000001){
		kprintf("Invalid task\n");
		return;
	}
	current_task_fcfs->status = RUNNING;
	start_task(current_task_fcfs->psp);
}

void create_task_fcfs(TCB_TypeDef *tcb, void(*task)(void), uint32_t *stack_start){
	
}

void __attribute__((naked)) PendSV_Handler(void){

	SCB->ICSR |= (1<<27);
	__schedule();

	__asm volatile(
		"mov r0, %0" 
		: 
		:"r"(current_task_fcfs->psp)
	);
	__asm volatile(
		"ldmia r0!,{r4-r11}\n"
		"msr psp, r0\n"
		"pop {lr}\n"
		"bx lr\n"
	);
}