#include <schedule.h>
ReadyQ_TypeDef queue;
TCB_TypeDef *current_task;
TCB_TypeDef*sleep_task;
uint32_t TASK_ID = 1000;
uint32_t exec_start_time = 0;


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
	current_task->execution_time += PER_TASK_TIME;
	current_task = pop();
	if(current_task->response_time_t == 0){
		current_task->response_time_t = __getTime();
	}
    current_task->status = RUNNING;
    return;
}


void __create_task(TCB_TypeDef *tcb, void(*task)(void), uint32_t *stack_start){
	tcb->magic_number = 0xFECABAA0;
    tcb->digital_sinature = 0x00000001;
    tcb->task_id = TASK_ID++;
    tcb->status = READY;
    tcb->execution_time = 0;
    tcb->waiting_time = 0;
    tcb->start_time_t = get_time();
    tcb->priority = 1;
    tcb->response_time_t = 0;
    
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
    uint32_t cur_time = get_time();
	current_task->response_time_t = cur_time;
	exec_start_time = cur_time;
	current_task->status = RUNNING;
	start_task(current_task->psp);
}

void __set_sleep(TCB_TypeDef *task){
	sleep_task = task;
	return;
}

//attribute = naked -> active
//attribute = weak -> not active

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

//-------------helper functions----------------
#define STOP 		1000000
#define TASK_COUNT 	10


TCB_TypeDef __task[MAX_TASK],__sleep;
uint32_t GLOBAL_COUNT = 0;

void task_1(void);
void sleep_state(void);
void scheduling_tester(void);
void print_task_time(void);
void task_1(void){
	uint32_t value;
	uint32_t inc_count=0;
	uint32_t pid = getpid();
	uint32_t st_time = get_time();
	kprintf("___________________Task %d___________________\n",pid);
	while(1){
		value = GLOBAL_COUNT;
		value++;
		if(value != GLOBAL_COUNT+1){ //we check is someother task(s) increase the count
			kprintf("Error %d != %d\n\r",value,GLOBAL_COUNT+1); /* It is an SVC call*/
		} else{
			GLOBAL_COUNT=value;
			inc_count++;
		}
		if(GLOBAL_COUNT >= STOP){
			kprintf("Total increment done by task %d is: %d\n\r",pid,inc_count);
			break;
		}
	}
	uint32_t turn_around_time = get_time() - st_time;

	TCB_TypeDef *this_task = (__task + pid - 1000);
	this_task->waiting_time = turn_around_time - this_task->execution_time;
	task_exit();
}
void sleep_state(void){
	__set_pending(0);
	print_task_time();
	kprintf("Sleeping\n");
	while(1);
}

void scheduling_tester(void){
	init_queue();
	
	for(int i = 0;i < TASK_COUNT;i++){
		__create_task((TCB_TypeDef *)__task + i,task_1,(uint32_t*)TASK_STACK_START - i * TASK_STACK_SIZE);
		queue_add((TCB_TypeDef *)__task + i);
	}
	
	__create_task((TCB_TypeDef *)&__sleep,sleep_state,(uint32_t*)TASK_STACK_START - TASK_COUNT * TASK_STACK_SIZE);
	__set_sleep((TCB_TypeDef *)&__sleep);
		
	__set_pending(1);
	start_exec();
	kprintf("___________END SCHEDULING TESTER___________\n");
}


void print_task_time(void){
	kprintf("_______________________________________________________________\n");

	for(int i = 0;i < TASK_COUNT;i++){
		TCB_TypeDef *task = (TCB_TypeDef *)__task + i;
		kprintf("Task ID: %d, Execution Time: %d,   Waiting Time: %d,   Response Time: %d   Turn Around Time: %d\n",
				task->task_id, task->execution_time, task->waiting_time, task->response_time_t, task->waiting_time + task->execution_time);
	}
	kprintf("_______________________________________________________________\n");	
}