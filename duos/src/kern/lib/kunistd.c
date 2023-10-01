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
#include <kunistd.h>
#include <usart.h>
#include <types.h>
#include<kstdio.h>
#include <cm4.h>
/* Add your functions here */
void __sys_write(unsigned int fd, char *str)
{
    switch (fd)
    {
    case STDOUT_FILENO:
        //kprintf("before usart %s\n",str);
        _USART_WRITE(USART2, str);
        break;
    
    default:
        break;
    }
}
void __sys_read(unsigned int fd, char** str)
{
    char ch[100];
    switch (fd)
    {
        case STDIN_FILENO:
            _USART_READ(USART2, (char*)ch, 1000);
            *str = ch;
            break;
    
        default:
            break;
    }
}

void __sys_reboot(void) 
{
	kprintf("CSEDU OS rebooting...");
    //2nd bit is SYS RESET REQUEST
	SCB->AIRCR = (0x05FA << 16) | (1 << 2);

	while(1);
}
void __sys_gettime(unsigned int *val)
{

	//__asm volatile ("mov %0, r2" : "=r" (t));
    //save value of t's address into R4
    //get the time from the given cm4 in starter code
	*val = __getTime();
	return;
}

void __sys_getpid(unsigned int *val,uint16_t value)
{
	*val = value;
	return ;
}
void __sys_exit(void)
{
    //TODO
    unsigned int *svc_args;
    __asm volatile("mov %0, r1" : "=r"(svc_args));
    //Save args pointer into r1
    //assign the task status as exit
    (*((TCB_TypeDef*)svc_args[4])).status = 4;
    return;
}

