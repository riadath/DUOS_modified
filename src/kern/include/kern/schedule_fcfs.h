#ifndef __SCHEDULE_FCFS_H
#define __SCHEDULE_FCFS_H


#define DUMMY_XPSR  0x01000000U

#define READY 1
#define RUNNING 2
#define TERMINATED 3
#define KILLED 4
#define STOP 		1000000
#define TASK_COUNT 	5
#define MAX_TASK 20

#include <types.h>
#include <stdint.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <unistd.h>
#include <task_queue.h>
#include <schedule.h>

extern uint32_t avg_response_time_fcfs;
extern uint32_t avg_waiting_time_fcfs;
extern uint32_t avg_execution_time_fcfs;
extern uint32_t avg_turnaround_time_fcfs;

void schedule_next_fcfs(void);
void start_exec_fcfs(void);
void create_task_fcfs(TCB_TypeDef *, void(*task)(void), uint32_t *);

void task_1_fcfs(void);
void process_stat_fcfs(void);
void scheduling_tester_fcfs(void);
void sleep_state_fcfs(void);



#endif