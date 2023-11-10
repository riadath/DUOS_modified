#ifndef __SEM_H
#define __SEM_H
#include <stdint.h>
#include <kstdio.h>
#include <schedule.h>
#include <kstring.h>
#include <types.h>
#include <task_queue.h>

// __asm__ volatile(
//     "EXPORT sem_dec             \n"
// "sem_dec PROC                   \n"
//     "1  LDREX   r1, [r0]        \n"
//     "   CMP     r1, #0          \n"
//     "   BEQ     %f2             \n"
//     "   SUB     r1, #1          \n"
//     "   STREX   r2, r1, [r0]    \n"
//     "   CMP     r2, #0          \n"
//     "   BNE     %b1             \n"
//     "   DMB                     \n"
//     "   BX      lr              \n"
//     "2                          \n"
//     "   WAIT_FOR_UPDATE         \n"
//     "   B       %b1             \n"
//     "   ENDP                    \n"
//     );


// __asm__ volatile(
//     "EXPORT sem_inc             \n"
// "sem_inc PROC                   \n"
//     "1  LDREX   r1, [r0]        \n"
//     "   ADD     r1, #1          \n"
//     "   STREX   r2, r1, [r0]    \n"
//     "   CMP     r2, #0          \n"
//     "   BNE     %b1             \n"
//     "   CMP     r0, #1          \n"
//     "   DMB                     \n"
//     "   BGE     %f2             \n"
//     "   BX      lr              \n"
//     "2                          \n"
//     "   SIGNAL_UPDATE           \n"
//     "   BX      lr              \n"
//     "   ENDP                    \n"
//     );

#endif