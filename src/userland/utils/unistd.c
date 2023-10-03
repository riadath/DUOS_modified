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

void fopen(char *name,uint8_t t_access, uint32_t *op_addr){
	__asm volatile (
		"mov r0, %[x]\n"
		"mov r1, %[y]\n"
		: 
		: [x] "r" (name), [y] "r" (t_access)
	);
	__asm volatile(
		"mov r2, %[x]\n"
		:
		: [x] "r" (op_addr)
	);

	__asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc #45\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );

}

void fclose(uint32_t *op_addr){
	__asm volatile (
		"mov r0, %[x]\n"
		:
		: [x] "r" (op_addr)
	);
	__asm volatile (
		"stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
		"svc #49\n"
		"ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
	);
}

void reboot(void){
	__asm volatile (
		"stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
		"svc #119\n"
		"ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
	);
}

void scanf(uint8_t fd,char **data,uint32_t size){
	__asm volatile (
		"mov r0, %[x]\n"
		"mov r1, %[y]\n"
		:
		: [x] "r" (fd), [y] "r" (data)
	);
	__asm volatile (
		"mov r2, %[x]\n"
		:
		: [x] "r" (size)
	);
	__asm volatile (
		"stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
		"svc #50\n"
		"ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
	);
}

/* Write your highlevel I/O details */

