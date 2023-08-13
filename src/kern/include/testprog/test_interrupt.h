#ifndef __TEST_INTERRUPT_H
#define __TEST_INTERRUPT_H

#include <gpio.h>
#include <cm4.h>
#include <sys.h>
#include <gpio.h>
#include <stm32_peps.h>


void blinky_test_code(void);

void test_enable_disable(void);
void test_set_unset(void);
void test_masking(void);

#endif