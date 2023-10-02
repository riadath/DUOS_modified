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

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>
#include <errmsg.h>
#include <kstdio.h>
#include <kunistd.h>
void syscall(uint32_t *svc_args)
{
/* The SVC_Handler calls this function to evaluate and execute the actual function */
/* Take care of return value or code */

	uint16_t callno = ((char *)svc_args[6])[-2];
	
	switch(callno)
	{
		/* Write your code to call actual function (kunistd.h/c or times.h/c and handle the return value(s) */
		case SYS_open:
		{
			kprintf("Will call __sys_open\n");
			char *device_name = (char *)svc_args[0];
			uint8_t t_access = (uint8_t)svc_args[1];
			uint32_t *op_addr = (uint32_t *)svc_args[2];
			__sys_open(device_name,t_access,op_addr);
			break;		
		}
		case SYS_close:
		{
			kprintf("Will call __sys_close\n");
			uint32_t *op_addr = (uint32_t *)svc_args[0];
			__sys_close(op_addr);
			break;
		}
		case SYS_read: 
			kprintf("Will call __sys_read\n");
			break;
		case SYS_write:
			kprintf("Will call __sys_write\n");
			break;
		case SYS_reboot:
			kprintf("Will call __sys_reboot\n");
			__sys_reboot();
			break;	
		case SYS__exit:
			kprintf("Will call __sys_exit\n");
			break;
		case SYS_getpid:
			kprintf("Will call __sys_getpid\n");
			break;
		case SYS___time:
			kprintf("Will call __sys_time\n");
			break;
		case SYS_yield:
			kprintf("Will call __sys_yield\n");
			break;				
		/* return error code see error.h and errmsg.h ENOSYS sys_errlist[ENOSYS]*/	
		default: ;
	}
/* Handle SVC return here */
}

