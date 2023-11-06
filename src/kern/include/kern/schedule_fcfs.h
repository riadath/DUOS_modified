#ifndef __SCHEDULE_FCFS_H
#define __SCHEDULE_FCFS_H


#define DUMMY_XPSR  0x01000000U

#define READY 1
#define RUNNING 2
#define TERMINATED 3
#define KILLED 4

#define MAX_TASK 20
#include <types.h>
#include <stdint.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <unistd.h>

void init_queue_fcfs(void);
void queue_add_fcfs(TCB_TypeDef *);
TCB_TypeDef* pop_fcfs(void);
void __schedule_fcfs(void);
void start_exec_fcfs(void);
void create_task_fcfs(TCB_TypeDef *tcb, void(*task)(void), uint32_t *stack_start);

#endif