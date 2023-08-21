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
	kprintf("Testing enable disable\n");
	//testing enable disable with exti0
	EXTI_GPIO_Config(GPIOA,0);
	EXTI_GPIO_Config(GPIOA,1);
	EXTI0_Init();
	EXTI1_Init();
	// __disable_irq();
	// __enable_irq();
	blinky_test_code();
	
}


void test_masking(void){
	//
	blinky_test_code();
}


