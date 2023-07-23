#include <led_matrix.h>
#include <gpio.h>
#include <cm4.h>
void test_code_pa5(void){

    int test_pin = 3;
    GPIO_InitTypeDef gpio_init;
    gpio_init.Mode = GPIO_MODE_OUTPUT;
    gpio_init.Pin = (1 << test_pin);
    GPIO_Init(GPIOB,&gpio_init);
    
    while(1){
        GPIO_WritePin(GPIOB,test_pin,1);
        // __SysTick_init(1000);	
        // while(__getTime() < 1000);
        // GPIO_WritePin(GPIOA,5,0);
        // __SysTick_init(1000);	
        // while(__getTime() < 1000);
    }
}