#include <sem.h>

extern void sem_inc(volatile uint32_t* semaphore);
extern void sem_dec(volatile uint32_t* semaphore);
unsigned int task_semaphore = 0;



void test_sem(void);
void test_sem(void) {
    sem_inc(&task_semaphore);
    kprintf("task_semaphore: %d\n", task_semaphore);
    sem_dec(&task_semaphore);
    kprintf("task_semaphore: %d\n", task_semaphore);
    kprintf("Testing semaphores\n");
}