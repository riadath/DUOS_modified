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
 
#include <cm4.h>
#include <clock.h>
#include <syscall.h>
volatile static uint32_t __mscount;
volatile static int pend_task = 0;
/************************************************************************************
* __SysTick_init(uint32_t reload) 
* Function initialize the SysTick clock. The function with a weak attribute enables 
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/

__attribute__((weak)) void __SysTick_init(uint32_t reload)
{
    SYSTICK->CTRL &= ~(1<<0); //disable systick timer
    SYSTICK->VAL =0; // initialize the counter
    __mscount=0;
    SYSTICK->LOAD = PLL_N*reload;
    SYSTICK->CTRL |= 1<<1 | 1<<2; //enable interrupt and internal clock source
    SYSTICK->CTRL|=1<<0; //enable systick counter
}

/************************************************************************************
* __sysTick_enable(void) 
* The function enables the SysTick clock if already not enabled. 
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/
__attribute__((weak)) void __sysTick_enable(void)
{
    if(SYSTICK->CTRL & ~(1<<0)) SYSTICK->CTRL |= 1<<0;
}
__attribute__((weak)) void __sysTick_disable(void)
{
    if(!(SYSTICK->CTRL & ~(1<<0))) SYSTICK->CTRL &= ~(1<<0);
}
__attribute__((weak)) uint32_t __getSysTickCount(void)
{
    return SYSTICK->VAL;
}
/************************************************************************************
* __updateSysTick(uint32_t count) 
* Function reinitialize the SysTick clock. The function with a weak attribute enables 
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/

__attribute__((weak)) void __updateSysTick(uint32_t count)
{
    SYSTICK->CTRL &= ~(1<<0); //disable systick timer
    SYSTICK->VAL =0; // initialize the counter
    __mscount=0;
    SYSTICK->CTRL |= 1<<1 | 1<<2; //enable interrupt and internal clock source
    SYSTICK->LOAD = PLL_N*count;
    SYSTICK->CTRL|=1<<0; //enable systick counter
}

/************************************************************************************
* __getTime(void) 
* Function return the SysTick elapsed time from the begining or reinitialing. The function with a weak attribute enables 
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/

__attribute__((weak)) uint32_t __getTime(void)
{
    return (__mscount+(SYSTICK->LOAD-SYSTICK->VAL)/(PLL_N*1000));
}
__attribute__((weak)) void SysTick_Handler()
{
    __mscount+=(SYSTICK->LOAD)/(PLL_N*1000);
    if(pend_task != 0)
        SCB->ICSR |= (1 << 28); // pendsv set pending bit
}

void set_task_pending(int value) 
{
    if(value == 0) {
        pend_task = 0;
    } else {
        pend_task = 1;
    }
}

void __enable_fpu()
{
    SCB->CPACR |= ((0xF<<20));
}

/**
 * old sys.c nvic related functions start from here
 * 
 * 
 * 
 *
**/

void SetPriorityGroup(uint32_t group) 
{
    SCB->AIRCR |= (0x5FA << 16) | (group << 24);
}

void __NVIC_SetPriority (IRQn_TypeDef IRQn, uint32_t priority)
{
    if(IRQn >= 0) {
        // setting priority via the interrupt priority register
        NVIC->ISPR[IRQn] |= ( priority << (4) );
    }
    else
    {
        switch (IRQn)
        {
        // mem management fault
        case -12:
            SCB->SHPR1 |= (uint8_t)(0xFFU & priority) << 0;
            break;
        // BusFault_IRQn
        case -11:
            SCB->SHPR1 |= (uint8_t)(0xFFU & priority) << 8;
            break;
        // UsageFault_IRQn
        case -10:
            SCB->SHPR1 |= (uint8_t)(0xFFU & priority) << 16;
            break;
        // SVCall_IRQn
        case -5:
            SCB->SHPR2 |= (uint8_t)(0xFFU & priority) << 24;
            break;
        // PendSV
        case -2:
            SCB->SHPR3 |= (uint8_t)(0xFFU & priority) << 16;
            break;
        // SysTick exception
        case -1:
            SCB->SHPR3 |= (uint8_t)(0xFFU & priority) << 24;
            break;
        default:
            return;
        }
    }
}

uint32_t __NVIC_GetPriority (IRQn_TypeDef IRQn) 
{
    uint32_t toReturn = 0;
    if(IRQn >= 0) {
        toReturn |= ( ( NVIC->ISPR[IRQn] >> 4 ) & 0xFF);
    }
    return toReturn;
}

void __NVIC_EnableIRQn(IRQn_TypeDef IRQn)
{
    if(IRQn >= 0) {
        int8_t index = IRQn / 32;
        int8_t position = IRQn % 32;
        // enables via the set enable register
        NVIC->ISER[index] |= (1 << position);
    }
} 

void __NVIC_DisableIRQn(IRQn_TypeDef IRQn)
{
    if(IRQn >= 0) {
        int8_t index = IRQn / 32;
        int8_t position = IRQn % 32;
        // disables via the clear enable register
        NVIC->ICER[index] &= ~( 1 << position );
    }
} 

uint32_t __NVIC_GetActive (IRQn_TypeDef IRQn)
{
    if(IRQn >= 0) {
        uint32_t index = IRQn / 32;
        uint32_t position = IRQn % 32;
        // returns active status of interrupt
        return (uint32_t) ( NVIC->IABR[index] & (uint32_t)( 1 << position ) );
    }
    return 0;
}

uint32_t __get_pending_IRQn(IRQn_TypeDef IRQn)
{
    if(IRQn >= 0) {
        uint32_t index = IRQn / 32;
        uint32_t position = IRQn % 32;        
        // returns the pending status of an interrupt, 1 = pending, 0 = not pending
        return (uint32_t) ( NVIC->ISPR[index] & (uint32_t)( 1 << position ) );
    }
    return 0;
}

void __clear_pending_IRQn(IRQn_TypeDef IRQn) 
{
    if(IRQn >= 0) {
        uint32_t index = IRQn / 32;
        uint32_t position = IRQn % 32;        
        // clears the interrupt pending bit
        NVIC->ICPR[index] &= ~( (uint32_t)(1u << position) );
    }
}

void __set_pending_IRQn(IRQn_TypeDef IRQn)
{
    if(IRQn >= 0) {
        uint32_t index = IRQn / 32;
        uint32_t position = IRQn % 32;        
        // clears the interrupt pending bit
        NVIC->ISPR[index] |= ( (uint32_t)(1u << position) );
    }

}

void __disable_irq(void)
{
    __asm volatile ("cpsid i" : : : "memory"); 
}


void __enable_irq()
{
    __asm volatile ("CPSIE i"); // clears primask == enables interrupts
}


void __set_BASEPRI(uint32_t basePri)
{
    basePri = basePri <<4;
    __asm volatile ("MSR basepri, %0" : : "r" (basePri) : "memory");
}

void __unset_BASEPRI(uint32_t value)
{
    // unmask interrupts greater or equal to the given argument/priority number
    __asm volatile ("MOVS R0, #0x0");
    __asm volatile ("MSR BASEPRI, R0");
    __set_BASEPRI(value);
}

void __set_PRIMASK(uint32_t priMask)
{
    __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}

uint32_t __get_PRIMASK(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, primask" : "=r" (result) );
    return (result);
}

void __enable_fault_irq(void)
{
    __asm volatile ("CPSIE f" : : : "memory");
}
void __set_FAULTMASK(uint32_t faultMask)
{
    __asm volatile ("MSR faultmask, %0" : : "r" (faultMask) : "memory");
}

void __disable_fault_irq(void)
{
    __asm volatile ("CPSID f" : : : "memory");  
}

uint32_t __get_FAULTMASK(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, faultmask" : "=r" (result) );
    return (result);
}

uint32_t __get_BASEPRI(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, basepri" : "=r" (result) );
    return (result>>4);
}



