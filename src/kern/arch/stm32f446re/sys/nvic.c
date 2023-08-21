#include <nvic.h>

/* Functions of NVIC */
void __NVIC_SetPriority (enum IRQn_TypeDef IRQn,uint8_t priority){
    //kprintf("setting IRQn %d to priority %d\n",IRQn,priority);
	if(IRQn >= 0){
		NVIC->IP[IRQn] = (uint8_t)((priority << 4));
        //kprintf("priority of IRQn %d is %d\n",IRQn,(uint8_t)(NVIC->IP[IRQn])); 
	}
}

uint8_t __NVIC_GetPriority(enum IRQn_TypeDef IRQn){
	if(IRQn >= 0){
        //kprintf("getting priority of IRQn %d is %d\n",IRQn,(uint8_t)(NVIC->IP[IRQn]));  
		return (uint8_t)(NVIC->IP[IRQn] >> 4);
	}
}

void __NVIC_EnableIRQn(enum IRQn_TypeDef IRQn){
    if(IRQn >= 0){
        NVIC -> ISER[IRQn/32] |= (1 << (IRQn%32));
    }
}

void __NVIC_DisableIRQn(enum IRQn_TypeDef IRQn){
    if(IRQn >= 0){
        NVIC -> ICER[IRQn/32] |= (1 << (IRQn%32));
    }
}

void __disable_irq(){
    asm("mov r0,#1");
    asm("msr primask,r0");
}

void __enable_irq(){
    asm("mov r0,#0");
    asm("msr primask,r0");
}

void __set_BASEPRI(uint32_t value){
    asm("mov r0, %0" : "=r" (value));
    asm("msr basepri, r0");
}


void __unset_BASEPRI(){
    asm("mov r0,#0");
    asm("msr basepri,r0");
}

void __set_PRIMASK(uint32_t priMask){
    __asm("mov r0, %[priMask]" 
    : [priMask] "=r" (priMask)
    );
    __asm("msr primask, r0");
}

uint32_t __get_PRIMASK(void){
    uint32_t value;
    asm("mrs r0,primask");
    asm("mov %0,r0":"=r"(value));
    return value;
}

void __enable_fault_irq(void){
    asm("mov r0, #1");
    asm("msr primask, r0");
}

void __disable_fault_irq(void){
    asm("mov r0, #0");
    asm("msr primask, r0");
}

void __set_FAULTMASK(uint32_t faultMask){
    asm("mov r0, %0" : "=r" (faultMask));
    asm("msr primask, r0");
}

uint32_t __get_FAULTMASK(void){
    uint32_t value;
    asm("mrs r0,faultmask");
    asm("mov %0,r0":"=r"(value));
    return value;
}

void __clear_pending_IRQn(enum IRQn_TypeDef IRQn){
    if(IRQn >= 0){
        NVIC -> ICPR[IRQn/32] |= (1 << (IRQn%32));
    }
}

uint32_t __get_pending_IRQn(enum IRQn_TypeDef IRQn){
    uint32_t pendingState;
    if(IRQn >= 0){
        int regNumber = IRQn/32;
        int offset = IRQn % 32;
        pendingState = NVIC->ICPR[regNumber] & (1 << regNumber);
        pendingState = pendingState >> 5;
        return pendingState;
    }
}

uint32_t __NVIC_GetActive(enum IRQn_TypeDef IRQn){
    uint32_t pendingState;
    if(IRQn >= 0){
        int regNumber = IRQn/32;
        int offset = IRQn % 32;
        pendingState = NVIC->IABR[regNumber] & (1 << regNumber);
        pendingState = pendingState >> 5;
        return pendingState;
    }
}


