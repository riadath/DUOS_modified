#include <test_interrupt.h>
static uint32_t GLOBAL_COUNT_EXTI0 = 0;




void blinky_test_code(void){
	uint16_t gpio_pin = 5;
    GPIO_InitTypeDef gpio_init = {1 << gpio_pin,GPIO_MODE_OUTPUT_PP,0,GPIO_SPEED_FREQ_VERY_HIGH,0};
    GPIO_Init(GPIOA,&gpio_init);
	while(1){
		GPIOA->BSRR |= 0x1U << gpio_pin;
		__delay_ms(500);
		GPIOA->BSRR |= 0x1U << (gpio_pin+16U);
		__delay_ms(500);
	}
}

void test_set_get_priority(void){
	for(int interupt = 12;interupt < 17;interupt++){
		for(uint8_t iprio = 12;iprio < 20;iprio++){
			kprintf("(Set)interrupt no : %d %d\n",interupt,iprio);
			__NVIC_SetPriority(interupt,iprio);
			kprintf("(Get)Interrupt no : %d %d\n",interupt,(uint8_t)__NVIC_GetPriority(interupt));
		}
	}
}

void test_enable_disable(void){
	//testing enable disable with exti0
	kprintf("Testing enable disable\n");
	EXTI_GPIO_Config(GPIOA,0);
	EXTI0_Init();
	// __NVIC_DisableIRQn(EXTI0_IRQn);
	// __NVIC_EnableIRQn(EXTI0_IRQn);
	__set_PRIMASK(0);
	kprintf("primask : %d\n",__get_PRIMASK());
	blinky_test_code();
	
}


void test_masking(void){
	//set fault mask
	__set_FAULTMASK(0);
	kprintf("fault mask : %d\n",__get_FAULTMASK());
	blinky_test_code();
}


void test_hardfault(void){
	//read an address 0x20CDCDCD
	uint32_t *ptr = (uint32_t *)0x20CDCDCD;
	uint32_t val = *ptr;
	kprintf("val : %d\n",val);
}

void test_sys_tick(void){
	//set systick
	// __NVIC_SetPriority(SysTick_IRQn,12);
	kprintf("Systick Priority>> : %d\n",__NVIC_GetPriority(SysTick_IRQn));
	__set_BASEPRI(2);
	for(int i = 0;i < 5;i++){
		kprintf("Systick i : %d\n",i);
		__delay_ms(1000);
	}

	while(1);
}