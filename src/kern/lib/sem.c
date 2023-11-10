#include <sem.h>

// sem_dec
// Declare for use from C as extern void sem_dec(void * semaphore);
void sem_dec(void* semaphore) {
    asm volatile(
        ".macro WAIT_FOR_UPDATE \n"
        "   WFI                 \n"
        ".endm                  \n"
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
        "   BX      lr                  \n"
        "2:                             \n"   // ; Take appropriate action while waiting for semaphore to be incremented
        
        "   B       1b                  \n"
        : [r0] "=r" (semaphore));
}

// sem_inc
// Declare for use from C as extern void sem_inc(void * semaphore);
void sem_inc(void* semaphore) {
    asm volatile(
        ".macro SIGNAL_UPDATE \n"
        ".endm                  \n"
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
        "    BX      lr                 \n"
        : [r0] "=r" (semaphore) : );
}


volatile uint32_t task_semaphore = 5;

void test_sem(void) {
    kprintf("Testing semaphores\n");

    sem_inc(&task_semaphore);
    kprintf("task_semaphore: %d\n", task_semaphore);

    sem_dec(&task_semaphore);
    kprintf("task_semaphore: %d\n", task_semaphore);

    kprintf("Done testing semaphores\n");
    return;
}