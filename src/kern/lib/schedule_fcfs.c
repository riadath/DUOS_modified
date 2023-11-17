#include <schedule_fcfs.h>

//sceduling for first come first serve


uint32_t TASK_ID_FCFS = 1000;
uint32_t exec_start_time_fcfs = 0;
uint32_t GLOBAL_COUNT_FCFS = 0;

uint32_t avg_response_time_fcfs = 0;
uint32_t avg_waiting_time_fcfs = 0;
uint32_t avg_execution_time_fcfs = 0;
uint32_t avg_turnaround_time_fcfs = 0;

TCB_TypeDef task_fcfs[MAX_TASK];


void create_task_fcfs(TCB_TypeDef* tcb, void(*task)(void), uint32_t* stack_start) {
	tcb->magic_number = 0xFECABAA0;
	tcb->digital_sinature = 0x00000001;
	tcb->task_id = TASK_ID_FCFS++;
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


void schedule_next_fcfs(void) {
	// first come first serve
	uint32_t time = __getTime();
	tcb_queue.current_task->execution_time += time - tcb_queue.current_task->response_time_t;
	tcb_queue.current_task = pop_task();
	tcb_queue.current_task->response_time_t = time;
	tcb_queue.current_task->waiting_time += time - exec_start_time_fcfs;

	tcb_queue.current_task->status = RUNNING;
	return;
}

void start_exec_fcfs(void) {

	if (tcb_queue.size == 0)return;
	tcb_queue.current_task = pop_task();


	if (tcb_queue.current_task->magic_number != 0xFECABAA0
		|| tcb_queue.current_task->digital_sinature != 0x00000001) {
		kprintf("Invalid task\n");
		return;
	}
	uint32_t cur_time = get_time();
	exec_start_time_fcfs = cur_time;
	tcb_queue.current_task->response_time_t = cur_time;
	tcb_queue.current_task->status = RUNNING;
	start_task(tcb_queue.current_task->psp);
}


//attribute = naked -> active
//attribute = weak -> not active
#ifdef RR
void __attribute__((naked)) PendSV_Handler(void) {
	SCB->ICSR |= (1 << 27);
	asm volatile(
		"stmdb r0!, {r4-r11}\n"
		"push {lr}\n"
		);

	next_task_fcfs();

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
#endif

// __________________SCHEUDLING TESTER__________________

void task_1_fcfs(void) {
	uint32_t value;
	uint32_t inc_count = 0;
	uint32_t pid = getpid();
	//default loop runs 100000000 times
	// uint32_t loop = 10000000;
	// while(loop--){
	// 	asm("nop");
	// }

	kprintf("________________________Task pid: %d\n\r", pid);
	while (1) {
		value = GLOBAL_COUNT_FCFS;
		value++;
		if (value != GLOBAL_COUNT_FCFS + 1) { //we check is someother task(s) increase the count
			kprintf("Error %d != %d\n\r", value, GLOBAL_COUNT_FCFS + 1); /* It is an SVC call*/
		}
		else {
			GLOBAL_COUNT_FCFS = value;
			inc_count++;
		}
		if (GLOBAL_COUNT_FCFS >= STOP + 100000) {
			kprintf("Total increment done by task %d is: %d\n\r", pid, inc_count);
			break;
		}
	}
	task_exit();

}

void sleep_state_fcfs(void) {
	process_stat_fcfs();
	print_performace_comp();
	kprintf("\n\nSleeping\n");
	kprintf("Do you want to reboot? (y/n)\n");
	char cmd;
	kscanf("%c", &cmd);
	if (cmd == 'y') {
		reboot();
	}
	else if(cmd == 'n') {
		kprintf("Reboot cancelled\n");
	}

	while (1);
}

void scheduling_tester_fcfs(void) {
	scheduling_algo = 1;

	kprintf("\n\n________START SCHEDULING TESTER FOR FCFS________\n");

	init_queue();

	for (int i = 0;i < TASK_COUNT;i++) {
		create_task_fcfs((TCB_TypeDef*)task_fcfs + i, task_1_fcfs,
			(uint32_t*)TASK_STACK_START - i * TASK_STACK_SIZE);
		push_task((TCB_TypeDef*)task_fcfs + i);
	}
	create_tcb((TCB_TypeDef*)&tcb_queue.sleep, sleep_state_fcfs,
		(uint32_t*)TASK_STACK_START - TASK_COUNT * TASK_STACK_SIZE);
	start_exec_fcfs();

}



void process_stat_fcfs(void) {
	kprintf("\n\n__________________Process Statistics (FCFS)__________________\n");

	for (int i = 0; i < TASK_COUNT; i++) {
		TCB_TypeDef* task = (TCB_TypeDef*)task_fcfs + i;

		avg_execution_time_fcfs += task->execution_time;
		avg_waiting_time_fcfs += task->waiting_time;
		avg_response_time_fcfs += task->response_time_t;
		avg_turnaround_time_fcfs += task->waiting_time + task->execution_time;

		kprintf("Task ID: %d, Execution Time: %d, Waiting Time: %d, Response Time: %d Turn Around Time: %d\n",
			task->task_id, task->execution_time, task->waiting_time, task->response_time_t, task->waiting_time + task->execution_time);
	}
}


void print_performace_comp(void) {
	kprintf("\n\n__________________Performance Comparison__________________\n\n");
	kprintf("Round Robin:\n");
	kprintf("Average Execution Time: %d\n", avg_execution_time_rr);
	kprintf("Average Waiting Time: %d\n", avg_waiting_time_rr);
	kprintf("Average Response Time: %d\n", avg_response_time_rr);
	kprintf("Average Turn Around Time: %d\n", avg_turnaround_time_rr);

	kprintf("\n\nFirst Come First Serve:\n");
	kprintf("Average Execution Time: %d\n", avg_execution_time_fcfs);
	kprintf("Average Waiting Time: %d\n", avg_waiting_time_fcfs);
	kprintf("Average Response Time: %d\n", avg_response_time_fcfs);
	kprintf("Average Turn Around Time: %d\n", avg_turnaround_time_fcfs);


}
