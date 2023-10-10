/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#include <unistd.h>
/* Write your highlevel I/O details */

void ok(char* args)
{
    __asm volatile("mov r5, %[v]": : [v] "r" (args));
    __asm volatile("stmdb r13!, {r5}");
    __asm volatile (
        // "nop\n"
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 55\n"
        "nop\n"
        // "blx lr\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
}

void write(char* args)
{
    //kprintf("%s\n",args);
    __asm volatile("mov r5, %[v]": : [v] "r" (args));
    __asm volatile("stmdb r13!, {r5}");
    __asm volatile (
        // "nop\n"
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 55\n"
        "nop\n"
        // "blx lr\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
}
void read(char **args)
{
    char *get = "temp";

    __asm volatile("mov r5, %[v]": : [v] "r" (&get));
    __asm volatile("stmdb r13!, {r5}");

    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 50\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );

    (*args) = get;
}

void reboot()
{
    __asm volatile("svc 119");
}

int get_time()
{
    unsigned int t;
    __asm volatile("mov r5, %[v]": : [v] "r" (&t));
    __asm volatile("stmdb r13!, {r5}");
    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 113\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
    

    return (int)t;
}
void yeild(void)
{
    __asm volatile("svc 120");
}

uint16_t getpid(void)
{
    unsigned int pid = 0;
    kprintf("original pid address %d\n",&pid);
    __asm volatile("mov r5, %[v]": : [v] "r" (&pid));
    __asm volatile("stmdb r13!, {r5}");
    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 5\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
    return (uint16_t) pid;
}
void exit(void)
{
    __asm volatile ("mov r12, r11");
    __asm volatile("stmdb r13!, {r5}");
    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 3\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
    yeild();
}
