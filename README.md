# Dhaka University Operating System (DUOS)

## Introduction

DUOS stands for Dhaka University Operating System, an experimental educational operating system designed primarily for use in the OS course assignments for CSE students at the University of Dhaka. Development of DUOS began in July 2022 and is continuously evolving. The initial credit for this project goes to the 25th batch of CSE, DU.

DUOS is developed for ARM processors and, more specifically, for Cortex-M4 v7 architecture. It currently supports 32-bit ARMv7-M processor-based microcontrollers, such as STM32F4xx. The long-term vision for DUOS includes expanding its features to support other ARM processors, including ARMv8 and ARMv9, across both 32-bit and 64-bit architectures.

The primary goals of DUOS are to provide a lightweight and educational operating system that serves as a development platform for controller designers and intelligent system developers. Additionally, DUOS aims to help technology enterprises deliver industry-grade control systems.

## Duos Directory Structure
duos
└── src
    ├── compile
    │   ├── Makefile
    │   ├── mapfiles
    │   ├── object
    │   └── target
    ├── doc
    │   └── Readme.txt
    └── kern
        ├── arch
        │   ├── cm4
        │   │   └── cm4.c
        │   ├── include
        │   │   └── cm4
        │   │       └── cm4.h
        │   └── stm32f446re
        │       ├── dev
        │       │   ├── clock.c
        │       │   ├── gpio.c
        │       │   ├── timer.c
        │       │   └── usart.c
        │       ├── include
        │       │   ├── dev
        │       │   │   ├── clock.h
        │       │   │   ├── gpio.h
        │       │   │   ├── timer.h
        │       │   │   └── usart.h
        │       │   └── sys
        │       │       ├── stm32_peps.h
        │       │       └── stm32_startup.h
        │       ├── linker
        │       │   └── linker.ld
        │       └── sys
        │           └── stm32_startup.c
        ├── dev
        ├── include
        │   ├── kern
        │   │   ├── errmsg.h
        │   │   ├── errno.h
        │   │   ├── syscall_def.h
        │   │   ├── sys_init.h
        │   │   └── unistd.h
        │   ├── kfloat.h
        │   ├── kmain.h
        │   ├── kmath.h
        │   ├── kstdio.h
        │   ├── kstring.h
        │   └── syscall.h
        ├── kmain
        │   └── kmain.c
        ├── lib
        │   ├── kfloat.c
        │   ├── kmath.c
        │   ├── kstdio.c
        │   ├── kstring.c
        │   └── sys_init.c
        ├── proc
        ├── syscall
        │   └── syscalls.c
        ├── thread
        └── vfs

