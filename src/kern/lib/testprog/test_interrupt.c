#include <test_interrupt.h>

void blinky_test_code(void){
	uint16_t gpio_pin = 0;
    GPIO_InitTypeDef gpio_init = {1 << gpio_pin,GPIO_MODE_OUTPUT_PP,0,GPIO_SPEED_FREQ_VERY_HIGH,0};
    GPIO_Init(GPIOA,&gpio_init);
	while(1){
		GPIOA->BSRR |= 0x1U << gpio_pin;
		__delay_ms(500);
		GPIOA->BSRR |= 0x1U << (gpio_pin+16U);
		__delay_ms(500);
	}
}

void test_enable_disable(void){
}

void test_set_unset(void){
}

void test_masking(void){
}


