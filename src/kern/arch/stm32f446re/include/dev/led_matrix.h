#ifndef __LED_MATRIX_H
#define __LED_MATRIX_H
#include "../sys/stm32_peps.h"

#define ROW_COUNT 3
#define COL_COUNT 3

typedef struct {
    GPIO_TypeDef* gpio;
    uint16_t pin;
}LED_PIN_INFO;

LED_PIN_INFO pin_info_table[ROW_COUNT][COL_COUNT] = {
    {{GPIOA,5},{GPIOA,6},{GPIOA,7}},
    {{GPIOA,8},{GPIOB,9},{GPIOA,10}},
    {{GPIOB,1},{GPIOB,2},{GPIOB,3}}
};



void test_code_pa5(void);

#endif