#ifndef __SEM_H
#define __SEM_H
#include <stdint.h>
#include <kstdio.h>
#include <schedule.h>
#include <kstring.h>
#include <types.h>
#include <task_queue.h>

void test_sem(void);
extern void sem_dec(void* semaphore);
extern void sem_inc(void* semaphore);


#endif