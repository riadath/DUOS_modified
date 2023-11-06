#include <schedule_fcfs.h>

//sceduling for first come first serve

ReadyQ_TypeDef queue_fcfs;
TCB_TypeDef *current_task_fcfs,*sleep_task_fcfs;
uint32_t TASK_ID_FCFS = 1000;

uint32_t exec_start_time_fcfs = 0;

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



void next_task_fcfs(void){
    // first come first serve
    uint32_t time = __getTime();
    current_task_fcfs->execution_time += time - exec_start_time_fcfs;
    current_task_fcfs = pop_fcfs();
    current_task_fcfs->response_time_t = time;
    current_task_fcfs->waiting_time += time - exec_start_time_fcfs;

    current_task_fcfs->status = RUNNING;
    return;
}

void set_sleep_fcfs(TCB_TypeDef *task){
    sleep_task_fcfs = task;
    return;
}

void start_exec_fcfs(void){
    
	if(queue_fcfs.size == 0)return;
	current_task_fcfs = pop_fcfs(); 


	if(current_task_fcfs->magic_number != 0xFECABAA0
	|| current_task_fcfs->digital_sinature != 0x00000001){
		kprintf("Invalid task\n");
		return;
	}
    uint32_t cur_time = get_time();
    exec_start_time_fcfs = cur_time;
    current_task_fcfs->response_time_t = cur_time;
	current_task_fcfs->status = RUNNING;
	start_task(current_task_fcfs->psp);
}

void create_task_fcfs(TCB_TypeDef *tcb, void(*task)(void), uint32_t *stack_start){
	tcb->magic_number = 0xFECABAA0;
    tcb->digital_sinature = 0x00000001;
    tcb->task_id = TASK_ID_FCFS++;
    tcb->status = READY;
    tcb->execution_time = 0;
    tcb->waiting_time = 0;
    tcb->start_time_t = get_time();
    tcb->priority = 1;
    tcb->start_time_t = 0;
    
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



void __attribute__((naked)) PendSV_Handler(void){
	SCB->ICSR |= (1<<27);
	//save current context
	__asm volatile(
		"mrs r0, psp\n"
		"stmdb r0!, {r4-r11}\n"
		"push {lr}\n"
	);

	__asm volatile("mov %0, r0" 
		: "=r" (current_task_fcfs->psp)
		:
	);
	next_task_fcfs();

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




void print_current_task(void){
    kprintf("Current task id: %d\n",current_task_fcfs->task_id);
    kprintf("Current task magic number: %x\n",current_task_fcfs->magic_number);
    return;
}

void print_all_queue(void){
   while(queue_fcfs.size != 0){
        TCB_TypeDef *task = pop_fcfs();
        
        kprintf("Task id: %d\n",task->task_id);
        kprintf("Task magic number: %x\n",task->magic_number);
    }
    return;
    
}