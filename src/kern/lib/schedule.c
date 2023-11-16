#include <schedule.h>
#include <sem.h>

volatile TaskQueue_TypeDef tcb_queue;
volatile uint32_t if_lock = 0;
TCB_TypeDef tcb_list[MAX_TASK];
uint32_t TASK_ID = 1000, exec_start_time = 0;


void wait(void) {
	asm volatile(
		"mov r0, %[x]\n"
		"bl sem_dec\n"
		"bx lr\n"
		:
	: [x] "r" (&task_semaphore)
		);
}

void signal(void) {
	asm volatile(
		"mov r0, %[x]\n"
		"bl sem_inc\n"
		"bx lr\n"
		:
	: [x] "r" (&task_semaphore)
		);
}



//-------------scheduling functions----------------
void schedule_next(void) {
	if (tcb_queue.current_task->status == RUNNING) {
		tcb_queue.current_task->status = READY;
		push_task(tcb_queue.current_task);
	}
	if(tcb_queue.current_task->status == SLEEPING){
		push_sem(tcb_queue.current_task);
	}
	//Timing calculation
	if (tcb_queue.current_task->status == KILLED) {
		uint32_t turn_around_time = __getTime() - tcb_queue.current_task->start_time_t;
		tcb_queue.current_task->waiting_time = turn_around_time - tcb_queue.current_task->execution_time;
	}
	tcb_queue.current_task->execution_time += PER_TASK_TIME;


	tcb_queue.current_task = pop_task();
	tcb_queue.current_task->status = RUNNING;
	if(tcb_queue.current_task->status == SLEEPING){
		return;
	}
	//Timing calculation
	if (tcb_queue.current_task->response_time_t == 0) {
		tcb_queue.current_task->response_time_t = __getTime();
	}


	return;
}


void start_exec(void) {
	if (tcb_queue.size == 0)return;
	tcb_queue.current_task = pop_task();
	if (tcb_queue.current_task->magic_number != 0xFECABAA0
		|| tcb_queue.current_task->digital_sinature != 0x00000001) {
		kprintf("Invalid task\n");
		return;
	}
	uint32_t cur_time = get_time();
	tcb_queue.current_task->response_time_t = cur_time;
	exec_start_time = cur_time;
	tcb_queue.current_task->status = RUNNING;
	start_task(tcb_queue.current_task->psp);
}

void create_tcb(TCB_TypeDef* tcb, void(*task)(void), uint32_t* stack_start) {
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
	for (int i = 0; i < 5; i++) *(--tcb->psp) = 0x00000000;
	*(--tcb->psp) = (uint32_t)tcb;
	//store R4 - R10
	for (int i = 0; i < 7; i++) *(--tcb->psp) = 0x00000000;
}

//attribute = naked -> active
//attribute = weak -> not active

void __attribute__((naked)) PendSV_Handler(void) {
	//Clear all pending interrupts
	SCB->ICSR |= (1 << 27);

	//save current context
	asm volatile(
		"mrs r0, psp\n"
		"stmdb r0!, {r4-r11}\n"
		"push {lr}\n"
		);

	asm volatile("mov %0, r0"
		: "=r" (tcb_queue.current_task->psp)
		:
		);
	//Schedule next task
	schedule_next();
	

	asm volatile(
		"mov r0, %0"
		:
	: "r"(tcb_queue.current_task->psp)
		);
	asm volatile(
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

void task_1(void) {
	// kprintf("<1 1___\n");
	uint32_t pid = 1;//getpid();
	// kprintf("<2 1___\n");
	uint32_t value, inc_count1 = 0;
	// kprintf("<3 1___\n");

	while (1) {
		// kprintf("<4 1___\n");

		// while(if_lock != 0);
		sem_dec(&task_semaphore); //increment semaphore (critical section)
		// if_lock = 1;

		// kprintf("<5 1___\n");
		value = GLOBAL_COUNT;
		// kprintf("<6 1___\n");
		value++;

		// kprintf("<7 1___\n");
		uint8_t is_valid = (value != GLOBAL_COUNT + 1);
		// kprintf("<8 1___\n");
		
		// while(if_lock != 0);
		sem_inc(&task_semaphore); //increment semaphore (critical section)
		// if_lock = 1;


		// kprintf("<9 1___\n");
		if (is_valid) {
		// kprintf("<10 1___\n");
			kprintf("Error in pid %d with %d != %d\n\r", pid, value, GLOBAL_COUNT + 1);
		// kprintf("<11 1___\n");
		}
		else {
		// kprintf("<12 1___\n");
			// sem_dec(&task_semaphore); //decrement semaphore (critical section)
			GLOBAL_COUNT = value;
		// kprintf("<13 1___\n");
			inc_count1++;
		// kprintf("<14 1___\n");
			// sem_inc(&task_semaphore); //increment semaphore (critical section)
		}
		// kprintf("<15 1___\n");
		// sem_dec(&task_semaphore); //decrement semaphore (critical section
		is_valid = (GLOBAL_COUNT >= STOP);
		// kprintf("<16 1___\n");
		// sem_inc(&task_semaphore); //increment semaphore (critical section)

		if (is_valid) {
		// kprintf("<17 1___\n");
			kprintf("Total increment done by task 1 is: %d\n\r", inc_count1);
		// kprintf("<18 1___\n");
			break;
		}
	}

	task_exit();
}
void task_2(void) {
	// kprintf("<1_2___\n");
	uint32_t pid = 2;//getpid();
	// kprintf("<2_2___\n");

	uint32_t value, inc_count2 = 0;
	// kprintf("<3_2___\n");

	while (1) {
		// kprintf("<4_2___\n");

		// while(if_lock != 1);
		sem_dec(&task_semaphore); //increment semaphore (critical section)
		// if_lock = 0;

		// kprintf("<5_2___\n");
		value = GLOBAL_COUNT;
		// kprintf("<6_2___\n");
		value++;
		// kprintf("<7_2___\n");
		uint8_t is_valid = (value != GLOBAL_COUNT + 1);
		// kprintf("<8_2___\n");

		// while(if_lock != 1);
		sem_inc(&task_semaphore); //increment semaphore (critical section)
		// if_lock = 0;

		// kprintf("<9_2___\n");
		if (is_valid) {
		// kprintf("<10_2___\n");
			kprintf("Error in pid %d with %d != %d\n\r", pid, value, GLOBAL_COUNT + 1);
		// kprintf("<11_2___\n");
		}
		else {
		// kprintf("<12_2___\n");
			// sem_dec(&task_semaphore); //decrement semaphore (critical section)
			GLOBAL_COUNT = value;
		// kprintf("<13_2___\n");
			inc_count2++;
		// kprintf("<14_2___\n");
			// sem_inc(&task_semaphore); //increment semaphore (critical section)
		}
		// kprintf("<15_2___\n");
		// sem_dec(&task_semaphore); //decrement semaphore (critical section
		is_valid = (GLOBAL_COUNT >= STOP);
		// kprintf("<16_2___\n");
		// sem_inc(&task_semaphore); //increment semaphore (critical section)

		if (is_valid) {
		// kprintf("<17_2___\n");
			kprintf("Total increment done by task 2 is: %d\n\r", inc_count2);
		// kprintf("<18_2___\n");
			break;
		}
	}

	task_exit();
}
void sleep_state(void) {
	set_pending(0);
	print_task_time();
	kprintf("Sleeping\n");
	while (1);
}


void scheduling_tester(void) {
	init_queue();
	// for (int i = 0; i < TASK_COUNT; i++) {
	// 	create_tcb((TCB_TypeDef*)tcb_list + i, task_1, (uint32_t*)TASK_STACK_START - i * TASK_STACK_SIZE);
	// 	push_task((TCB_TypeDef*)tcb_list + i);
	// }
	create_tcb(tcb_list, task_1, (uint32_t*)TASK_STACK_START);
	create_tcb(tcb_list + 1, task_2, (uint32_t*)(TASK_STACK_START - TASK_STACK_SIZE));
	push_task(tcb_list);
	push_task(tcb_list + 1);


	create_tcb((TCB_TypeDef*)&tcb_queue.sleep, sleep_state, (uint32_t*)(TASK_STACK_START - 2 * TASK_STACK_SIZE));
	set_pending(1);
	start_exec();
	kprintf("________END SCHEDULING TESTER________\n");
}

void print_task_time(void) {
	

	for (int i = 0;i < TASK_COUNT;i++) {
		TCB_TypeDef* task = (TCB_TypeDef*)tcb_list + i;
		kprintf("Task ID: %d, Execution Time: %d,   Waiting Time: %d,   Response Time: %d   Turn Around Time: %d\n",
			task->task_id, task->execution_time, task->waiting_time, task->response_time_t, task->waiting_time + task->execution_time);
	}
	
}


void print_task_info(void);
void print_task_info(void) {
	kprintf("----------------TASK Queue--------------------\n");

	for (int i = 0;i < TASK_COUNT;i++) {
		TCB_TypeDef* task = (TCB_TypeDef*)tcb_list + i;
		char status[20];
		if (task->status == 1) {
			__strcpy(status, "READY");
		}
		else if (task->status == 2) {
			__strcpy(status, "RUNNING");
		}
		else if (task->status == 3) {
			__strcpy(status, "TERMINATED");
		}
		else if (task->status == 4) {
			__strcpy(status, "KILLED");
		}
		else if (task->status == 5) {
			__strcpy(status, "SLEEPING");
		}
		// kprintf("<task id: %d, status: %s\n", task->task_id, status);
	}
}

void print_task_sem_queue(void) {
	kprintf("------------------SEM Queue---------------------\n");
	for (int i = 0;i < sem_queue.size;i++) {
		TCB_TypeDef* task = sem_queue.q[i];
		char status[20];
		if (task->status == 1) {
			__strcpy(status, "READY");
		}
		else if (task->status == 2) {
			__strcpy(status, "RUNNING");
		}
		else if (task->status == 3) {
			__strcpy(status, "TERMINATED");
		}
		else if (task->status == 4) {
			__strcpy(status, "KILLED");
		}
		else if (task->status == 5) {
			__strcpy(status, "SLEEPING");
		}
		kprintf("task id: %d, status: %s\n", task->task_id, status);
	}
}