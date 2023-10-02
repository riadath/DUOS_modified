#ifndef __DEV_TABLE_H
#define __DEV_TABLE_H
#include <stdint.h>
#include <kstdio.h>
#include <kstring.h>
#include <stm32_peps.h>

#define O_RDONLY 0
#define O_WRONLY 1
#define O_APPEND 2

typedef struct dev_t{
	char name[32]; // Device name or symbol
	uint32_t t_ref; //Number of open count
	uint8_t t_access; //open type O_RDONLY, O_WRDONLY, O_APPEND
	uint32_t *op_addr; //Address of the datastructure operations
}dev_table;

extern dev_table device_list[64];
extern uint32_t device_count;

void __init_dev_table(void);
#endif 

