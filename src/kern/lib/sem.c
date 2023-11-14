#include <sem.h>

volatile uint32_t task_semaphore = 1;
volatile WaitingQueue_TypeDef sem_queue;

void add_to_sem_queue(void) {
    if (tcb_queue.current_task->status == KILLED) {
        asm volatile("bx lr");
    }
    tcb_queue.current_task->status = SLEEPING;
    push_sem(tcb_queue.current_task);
    asm volatile("bx lr");
    // //set all task_except currnt one to sleep and push to sem_queue
    // for(int i = 0; i < tcb_queue.size; i++){
    //     if(tcb_queue.q[i]->status != KILLED || tcb_queue.q[i]->status != RUNNING){
    //         tcb_queue.q[i]->status = SLEEPING;
    //         push_sem(tcb_queue.q[i]);
    //     }
    // }
    // asm volatile("bx lr");
}

void rmv_from_sem_queue(void) {
    if (is_sem_empty()) {
        asm volatile("bx lr");
    }
    TCB_TypeDef* task = pop_sem();
    task->status = RUNNING;
    asm volatile("bx lr");
    //set all task in sem_queue to ready (pop from sem_queue)
    // while(!is_sem_empty()){
    //     TCB_TypeDef* task = pop_sem();
    //     task->status = READY;
    // }
    // asm volatile("bx lr");
}

void sem_dec(uint32_t* semaphore) {
    
    asm volatile(
        ".macro WAIT_FOR_UPDATE         \n"
        "   BL     add_to_sem_queue     \n"
        "   WFI                         \n"
        ".endm                          \n"
        );

    asm volatile (
        "1: LDREX   r1, [r0]            \n"
        "   CMP	    r1, #0              \n"   // ; Test if semaphore holds the value 0
        "   BEQ     2f                  \n"   // ; If it does, block before retrying
        "   SUB     r1, #1              \n"   // ; If not, decrement temporary copy
        "   STREX   r2, r1, [r0]        \n"   // ; Attempt Store-Exclusive
        "   CMP     r2, #0              \n"   // ; Check if Store-Exclusive succeeded
        "   BNE     1b                  \n"   // ; If Store-Exclusive failed, retry from start
        "   DMB                         \n"   // ; Required before accessing protected resource
        "   BX      lr                  \n"   // ; If Store-Exclusive succeeded, test if semaphore was 0
        "2:                             \n"   // ; Take appropriate action while waiting for semaphore to be incremented
        "   WAIT_FOR_UPDATE             \n"
        "   B       1b                  \n"
        : [r0] "=r" (semaphore) : );
}

void sem_inc(uint32_t* semaphore) {
    asm volatile(
        ".macro SIGNAL_UPDATE           \n"
        "    BL      rmv_from_sem_queue \n"
        "    DSB                        \n"
        "    SEV                        \n"
        ".endm                          \n"
        );
    asm volatile (
        "1:  LDREX   r1, [r0]           \n"
        "    ADD     r1, #1             \n"   // ; Increment temporary copy
        "    STREX   r2, r1, [r0]       \n"   // ; Attempt Store-Exclusive
        "    CMP     r2, #0             \n"   // ; Check if Store-Exclusive succeeded
        "    BNE     1b                 \n"   // ; Store failed - retry immediately
        "    CMP     r0, #1             \n"   // ; Store successful - test if incremented from zero
        "    DMB                        \n"   // ; Required before releasing protected resource
        "    BGE     2f                 \n"   // ; If initial value was 0, signal update
        "    BX      lr                 \n"
        "2:                             \n"   // ; Signal waiting processors or processes
        "    SIGNAL_UPDATE              \n"
        : [r0] "=r" (semaphore) : );
}




void print_all_sem_queue(void);
void print_all_sem_queue(void) {
    kprintf("sem_queue.size = %d\n", sem_queue.size);
    for (int i = 0; i < sem_queue.size; i++) {
        kprintf("sem_queue[%d] = %d\n", i, sem_queue.q[i]->task_id);
    }
}






