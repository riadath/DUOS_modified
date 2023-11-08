#ifndef __TASK_QUEUE_H
#define __TASK_QUEUE_H

#include <types.h>

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


#endif