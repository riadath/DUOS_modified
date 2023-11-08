#include <schedule.h>

TCB_TypeDef tcb_list[MAX_TASK];


uint32_t TASK_ID = 1000;
uint32_t exec_start_time = 0;


//-------------scheduling functions----------------

void schedule_next(void){
    if(tcb_queue.current_task->status == RUNNING){
        tcb_queue.current_task->status = READY;
        push_task(tcb_queue.current_task);
    }
	tcb_queue.current_task->execution_time += PER_TASK_TIME;
	tcb_queue.current_task = pop_task();
	if(tcb_queue.current_task->response_time_t == 0){
		tcb_queue.current_task->response_time_t = __getTime();
	}
    tcb_queue.current_task->status = RUNNING;
    return;
}


void create_tcb(TCB_TypeDef *tcb, void(*task)(void), uint32_t *stack_start){
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
	if(tcb_queue.size == 0)return;
	tcb_queue.current_task = pop_task(); 
	if(tcb_queue.current_task->magic_number != 0xFECABAA0
	|| tcb_queue.current_task->digital_sinature != 0x00000001){
		kprintf("Invalid task\n");
		return;
	}
    uint32_t cur_time = get_time();
	tcb_queue.current_task->response_time_t = cur_time;
	exec_start_time = cur_time;
	tcb_queue.current_task->status = RUNNING;
	start_task(tcb_queue.current_task->psp);
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
		: "=r" (tcb_queue.current_task->psp)
		:
	);
	//Schedule next task
	schedule_next();

	__asm volatile(
		"mov r0, %0" 
		: 
		:"r"(tcb_queue.current_task->psp)
	);
	__asm volatile(
		"ldmia r0!,{r4-r11}\n"
		"msr psp, r0\n"
		"pop {lr}\n"
		"bx lr\n"
	);
}

//-------------helper functions----------------

uint32_t GLOBAL_COUNT = 0;

void task_1(void);
void sleep_state(void);
void scheduling_tester(void);
void print_task_time(void);

void task_1(void)
{
	uint32_t value;
	uint32_t inc_count = 0;
	uint32_t pid = getpid();
	uint32_t st_time = get_time();
	kprintf("_________________tcb_list %d___________________\n\n", pid);
	while (1)
	{
		value = GLOBAL_COUNT;
		value++;
		if (value != GLOBAL_COUNT + 1)
		{															// we check is someother task(s) increase the count
			kprintf("Error %d != %d\n\r", value, GLOBAL_COUNT + 1); /* It is an SVC call*/
		}
		else
		{
			GLOBAL_COUNT = value;
			inc_count++;
		}
		if (GLOBAL_COUNT >= STOP)
		{
			kprintf("Total increment done by task %d is: %d\n\r", pid, inc_count);
			break;
		}
	}
	uint32_t turn_around_time = get_time() - st_time;

	TCB_TypeDef *this_task = (tcb_list + pid - 1000);
	this_task->waiting_time = turn_around_time - this_task->execution_time;
	task_exit();
}
void sleep_state(void)
{
	__set_pending(0);
	print_task_time();
	kprintf("Sleeping\n");
	while (1);
}


void scheduling_tester(void)
{
	init_queue();

	for (int i = 0; i < TASK_COUNT; i++)
	{
		create_tcb((TCB_TypeDef *)tcb_list + i, task_1, (uint32_t *)TASK_STACK_START - i * TASK_STACK_SIZE);
		push_task((TCB_TypeDef *)tcb_list + i);
	}
	create_tcb((TCB_TypeDef *)&tcb_queue.sleep, sleep_state, (uint32_t *)TASK_STACK_START - TASK_COUNT * TASK_STACK_SIZE);

	__set_pending(1);
	start_exec();
	kprintf("___________END SCHEDULING TESTER___________\n");
}

void print_task_time(void){
	kprintf("_______________________________________________________________\n");

	for(int i = 0;i < TASK_COUNT;i++){
		TCB_TypeDef *task = (TCB_TypeDef *)tcb_list + i;
		kprintf("Task ID: %d, Execution Time: %d,   Waiting Time: %d,   Response Time: %d   Turn Around Time: %d\n",
				task->task_id, task->execution_time, task->waiting_time, task->response_time_t, task->waiting_time + task->execution_time);
	}
	kprintf("_______________________________________________________________\n");	
}