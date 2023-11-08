// #include <schedule_fcfs.h>

// //sceduling for first come first serve

// TaskQueue_TypeDef queue_fcfs;
// TCB_TypeDef *current_task_fcfs;
// TCB_TypeDef *sleep_task_fcfs;
// uint32_t TASK_ID_FCFS = 1000;

// uint32_t exec_start_time_fcfs = 0;

// void init_queue_fcfs(void){
//     queue_fcfs.size = 0;
//     queue_fcfs.max = MAX_TASKS;
//     queue_fcfs.st = 0;
//     queue_fcfs.ed = -1;
// }

// void queue_add_fcfs(TCB_TypeDef *task){
// 	if (queue_fcfs.size == queue_fcfs.max){
// 		return;
// 	}
// 	queue_fcfs.ed = (queue_fcfs.ed + 1) % queue_fcfs.max;
// 	queue_fcfs.q[queue_fcfs.ed] = task;
// 	queue_fcfs.size++;
// }

// TCB_TypeDef* pop_fcfs(){
// 	if (queue_fcfs.size == 0){
// 		queue_add_fcfs(sleep_task_fcfs);
// 	}

// 	TCB_TypeDef *task = queue_fcfs.q[queue_fcfs.st];
// 	queue_fcfs.st = (queue_fcfs.st + 1) % queue_fcfs.max;
// 	queue_fcfs.size--;
// 	return task;
// }



// void next_task_fcfs(void){
//     // first come first serve
//     uint32_t time = __getTime();
//     current_task_fcfs->execution_time += time - current_task_fcfs->start_time_t;
//     current_task_fcfs = pop_fcfs();
//     current_task_fcfs->response_time_t = time;
//     current_task_fcfs->waiting_time += time - exec_start_time_fcfs;

//     current_task_fcfs->status = RUNNING;
//     return;
// }

// void set_sleep_fcfs(TCB_TypeDef *task){
//     sleep_task_fcfs = task;
//     return;
// }

// void start_exec_fcfs(void){
    
// 	if(queue_fcfs.size == 0)return;
// 	current_task_fcfs = pop_fcfs(); 


// 	if(current_task_fcfs->magic_number != 0xFECABAA0
// 	|| current_task_fcfs->digital_sinature != 0x00000001){
// 		kprintf("Invalid task\n");
// 		return;
// 	}
//     uint32_t cur_time = get_time();
//     exec_start_time_fcfs = cur_time;
//     current_task_fcfs->response_time_t = cur_time;
// 	current_task_fcfs->status = RUNNING;
// 	start_task(current_task_fcfs->psp);
// }

// void create_task_fcfs(TCB_TypeDef *tcb, void(*task)(void), uint32_t *stack_start){
// 	tcb->magic_number = 0xFECABAA0;
//     tcb->digital_sinature = 0x00000001;
//     tcb->task_id = TASK_ID_FCFS++;
//     tcb->status = READY;
//     tcb->execution_time = 0;
//     tcb->waiting_time = 0;
//     tcb->start_time_t = get_time();
//     tcb->priority = 1;
//     tcb->response_time_t = 0;
    
//     tcb->psp = stack_start;
// 	*(--tcb->psp) = DUMMY_XPSR; //xPSR
// 	*(--tcb->psp) = (uint32_t)task; //PC
// 	*(--tcb->psp) = 0xFFFFFFFD; //LR
	
// 	//store R0 - R3, R12
// 	for(int i = 0; i < 5; i++) *(--tcb->psp) = 0x00000000;
// 	*(--tcb->psp) = (uint32_t)tcb;
// 	//store R4 - R10
// 	for(int i = 0; i < 7; i++) *(--tcb->psp) = 0x00000000;
// }


// //attribute = naked -> active
// //attribute = weak -> not active

// void __attribute__((weak)) PendSV_Handler(void){
// 	SCB->ICSR |= (1<<27);
// 	__asm volatile(
// 		"stmdb r0!, {r4-r11}\n"
// 		"push {lr}\n"
// 	);
    
// 	next_task_fcfs();

// 	__asm volatile(
// 		"mov r0, %0" 
// 		: 
// 		:"r"(current_task_fcfs->psp)
// 	);
// 	__asm volatile(
// 		"ldmia r0!,{r4-r11}\n"
// 		"msr psp, r0\n"
// 		"pop {lr}\n"
// 		"bx lr\n"
// 	);
// }


// // __________________SCHEUDLING TESTER__________________

// #define STOP 		1000000
// #define TASK_COUNT 	5
// TCB_TypeDef task_fcfs[MAX_TASK],sleep_fcfs;
// uint32_t GLOBAL_COUNT_FCFS = 0;


// void task_1_fcfs(void);
// void sleep_state_fcfs(void);
// void print_all_task(void);
// void print_task_timing_data(void);
// void scheduling_tester_fcfs(void);


// void task_1_fcfs(void){
// 	uint32_t value;
// 	uint32_t inc_count=0;
// 	uint32_t pid = getpid();
// 	//default loop runs 100000000 times
// 	uint32_t loop = 10000000;
// 	while(loop--){
// 		__asm("nop");
// 	}

// 	kprintf("___________________Task FCFS %d___________________\n",pid);
// 	while(1){
// 		value = GLOBAL_COUNT_FCFS;
// 		value++;
// 		if(value != GLOBAL_COUNT_FCFS+1){ //we check is someother task(s) increase the count
// 			kprintf("Error %d != %d\n\r",value,GLOBAL_COUNT_FCFS+1); /* It is an SVC call*/
// 		} else{
// 			GLOBAL_COUNT_FCFS=value;
// 			inc_count++;
// 		}
// 		if(GLOBAL_COUNT_FCFS >= STOP){
// 			kprintf("Total increment done by task %d is: %d\n\r",pid,inc_count);
// 			break;
// 		}
// 	}
// 	task_exit();

// }

// void sleep_state_fcfs(void){
// 	print_task_timing_data();
// 	kprintf("Sleeping\n");
// 	while(1);
// }

// void print_all_task(void){
// 	kprintf("___________________\n");
// 	for(int i = 0;i < TASK_COUNT;i++){
// 		kprintf("task id %d\n",((TCB_TypeDef *)task_fcfs + i)->task_id);
// 		kprintf("task magic number %x\n",((TCB_TypeDef *)task_fcfs + i)->magic_number);
// 		kprintf("task digital sinature %x\n",((TCB_TypeDef *)task_fcfs + i)->digital_sinature);
// 		kprintf("task psp %x\n",((TCB_TypeDef *)task_fcfs + i)->psp);
// 	}
// 	kprintf("___________________\n");
// }

// void print_task_timing_data(void) {
//     kprintf("_______________________________________________________________\n");

//     for (int i = 0; i < TASK_COUNT; i++) {
//         TCB_TypeDef* task = (TCB_TypeDef*)task_fcfs + i;
//         kprintf("Task ID: %d, Execution Time: %d, Waiting Time: %d, Response Time: %d\n",
//                 task->task_id, task->execution_time, task->waiting_time, task->response_time_t);
//     }
//     kprintf("_______________________________________________________________\n");
// }


// void scheduling_tester_fcfs(void){
// 	init_queue_fcfs();
	
// 	for(int i = 0;i < TASK_COUNT;i++){
// 		create_task_fcfs((TCB_TypeDef *)task_fcfs + i,task_1_fcfs,(uint32_t*)TASK_STACK_START - i * TASK_STACK_SIZE);
// 		queue_add_fcfs((TCB_TypeDef *)task_fcfs + i);
// 	}

// 	create_task_fcfs((TCB_TypeDef *)&sleep_fcfs,sleep_state_fcfs,(uint32_t*)TASK_STACK_START - TASK_COUNT * TASK_STACK_SIZE);
// 	set_sleep_fcfs((TCB_TypeDef *)&sleep_fcfs);
	
// 	// print_all_queue();
// 	// print_all_task();
    
// 	start_exec_fcfs();
	
// 	kprintf("___________END FCFS SCHEDULING TESTER___________\n");
// }


// //for degugging
// void print_all_queue(void);
// void print_current_task(void);
// void print_current_task(void){
//     kprintf("Current task id: %d\n",current_task_fcfs->task_id);
//     kprintf("Current task magic number: %x\n",current_task_fcfs->magic_number);
//     return;
// }
// void print_all_queue(void){
//    while(queue_fcfs.size != 0){
//         TCB_TypeDef *task = pop_fcfs();
        
//         kprintf("Task id: %d\n",task->task_id);
//         kprintf("Task magic number: %x\n",task->magic_number);
//     }
//     return;
    
// }