#include <task_queue.h>

TaskQueue_TypeDef tcb_queue;

void init_queue(void){
	tcb_queue.size = 0;
	tcb_queue.max = MAX_TASKS;
	tcb_queue.st = 0;
	tcb_queue.ed = -1;
}

void push_task(TCB_TypeDef *task){
	if (tcb_queue.size == tcb_queue.max){
		return;
	}
	tcb_queue.ed = (tcb_queue.ed + 1) % tcb_queue.max;
	tcb_queue.q[tcb_queue.ed] = task;
	tcb_queue.size++;
}
TCB_TypeDef* pop_task(){
	if (tcb_queue.size == 0){
		push_task(&tcb_queue.sleep);
	}

	TCB_TypeDef *task = tcb_queue.q[tcb_queue.st];
	tcb_queue.st = (tcb_queue.st + 1) % tcb_queue.max;
	tcb_queue.size--;
	return task;
}

