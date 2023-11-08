#ifndef __TASK_QUEUE_H
#define __TASK_QUEUE_H

#include <types.h>
#include <schedule.h>
typedef struct ready_queue
{
	int size;
	int max;
	int st;
	int ed;
	TCB_TypeDef *q[30];
	TCB_TypeDef *current_task;
	TCB_TypeDef sleep;
} TaskQueue_TypeDef;



extern TaskQueue_TypeDef tcb_queue;


void init_queue(void);
void push_task(TCB_TypeDef *task);
TCB_TypeDef *pop_task(void);

#endif