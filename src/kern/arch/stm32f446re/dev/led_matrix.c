#include <led_matrix.h>
#include <gpio.h>
#include <cm4.h>
#include <stdbool.h>
void LED_MATRIX_INIT(void){
    GPIO_InitTypeDef gpio_init;
    
    for(uint8_t i = 0;i < ROW_COUNT;i++){
        for(uint8_t j = 0;j < COL_COUNT;j++){
            gpio_init.Mode = GPIO_MODE_OUTPUT;
            gpio_init.Pin = 1 << pin_info_table[i][j].pin;
            GPIO_Init(pin_info_table[i][j].gpio,&gpio_init);
        }        
    }
}

bool print_cond(uint8_t i,uint8_t j,uint8_t dig){
    switch(dig){
        case 0:
            if ((i == 1 && j == 1)){
                return false;
            }else{
                return true;
            }
            break;
        case 1:
            if (j == 1){
                return true;
            }else{
                return false;
            }
            break;
        case 2:
            if ((i == 0 && j == 0) || (i == 2 && j == 2)){
                return false;
            }else{
                return true;
            }
            break;
        case 3:
            if ((i == 1 && j == 0)){
                return false;
            }else{
                return true;
            }
            break;
        case 4:
            if ((i == 1) || (i == 0 && j == 0) || (i == 2 && j == 2)){
                return true;
            }else{
                return false;
            }
            break;
        case 5:
            if ((i == 0 && j == 2)){
                return false;
            }else{
                return true;
            }
            break;
        case 6:
            if ((i == 0 && j == 1) || (i == 0 && j == 2)){
                return false;
            }else{
                return true;
            }
            break;
        case 7:
            if ((i == 0) || (i > 0 && j == 2)){
                return true;
            }else{
                return false;
            }
            break;
        case 8:
            return true;
            break;
        case 9:
            if ((i < 2) || (i == 2 && j == 2)){
                return true;
            }else{
                return false;
            }
            break;
    }
}


void print_digit(uint8_t dig){
    for(uint8_t i = 0;i < ROW_COUNT;i++){
        for(uint8_t j = 0;j < COL_COUNT;j++){
            if(print_cond(i,j,dig)){
                GPIO_WritePin(pin_info_table[i][j].gpio,pin_info_table[i][j].pin,1);
            }else{
                GPIO_WritePin(pin_info_table[i][j].gpio,pin_info_table[i][j].pin,0);
            }
        }
    }
}


