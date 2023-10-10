# Makefile for DUOS: Last Updated on October 28, 2022
TOP=../kern
MCU=stm32f446re
CC=arm-none-eabi-gcc
MACH=cortex-m4
TARGET = target
FPU = -mfloat-abi=softfp -mfpu=fpv4-sp-d16
OBJECT = object
USERLAND = ../userland
INCLUDE=-I $(TOP)/arch/include -I $(TOP)/arch/$(MCU)/include -I $(TOP)/include -I $(TOP)/include/kern -I $(TOP)/arch/$(MCU)/include -I $(TOP)/arch/$(MCU)/include/dev -I $(TOP)/arch/include/cm4 -I $(TOP)/arch/$(MCU)/include/sys -I $(USERLAND)/include
MAPDIR = mapfiles
CFLAGS=-c -mcpu=$(MACH) -mthumb $(FPU) -std=gnu11 -Wall -O0
LDFLAGS = -nostdlib -nostartfiles -nodefaultlibs -fno-exceptions  -mcpu=$(MACH) -mthumb $(FPU) -T $(TOP)/arch/$(MCU)/linker/linker.ld -Wl,-Map=$(MAPDIR)/asst03.map -O0

all: asst03.elf

# lib set
LIBDIR = $(TOP)/lib
SRC_LIB = $(wildcard $(LIBDIR)/*.c)
LIB_OBJECTS = $(patsubst $(LIBDIR)/%.c, $(OBJECT)/%.o,$(SRC_LIB))

$(LIB_OBJECTS): $(OBJECT)/%.o: $(LIBDIR)/%.c
	$(info in lib set)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

# kmain set
MAINDIR = $(TOP)/kmain
SRC_MAIN = $(wildcard $(MAINDIR)/*.c)
MAIN_OBJECTS = $(patsubst $(MAINDIR)/%.c, $(OBJECT)/%.o,$(SRC_MAIN))

$(MAIN_OBJECTS): $(OBJECT)/%.o: $(MAINDIR)/%.c
	$(info in kmain set)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

# arch/board/dev set 
DEVDIR = $(TOP)/arch/$(MCU)/dev
SRC_DEV = $(wildcard $(DEVDIR)/*.c)
DEV_OBJECTS = $(patsubst $(DEVDIR)/%.c, $(OBJECT)/%.o,$(SRC_DEV))

$(DEV_OBJECTS): $(OBJECT)/%.o: $(DEVDIR)/%.c
	$(info in arch/board/dev set $(SRC_DEV))
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

# arch/board/sys set 
SYSDIR = $(TOP)/arch/$(MCU)/sys
SRC_SYS = $(wildcard $(SYSDIR)/*.c)
SYS_OBJECTS = $(patsubst $(SYSDIR)/%.c, $(OBJECT)/%.o,$(SRC_SYS))

$(SYS_OBJECTS): $(OBJECT)/%.o: $(SYSDIR)/%.c
	$(info in arch/board/sys set)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

# arch/cm4 set 
CM4DIR = $(TOP)/arch/cm4
SRC_CM4 = $(wildcard $(CM4DIR)/*.c)
CM4_OBJECTS = $(patsubst $(CM4DIR)/%.c, $(OBJECT)/%.o,$(SRC_CM4))

$(CM4_OBJECTS): $(OBJECT)/%.o: $(CM4DIR)/%.c
	$(info in arch/cm4 set)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^


# syscalls set 
SYSCALLSDIR = $(TOP)/syscall
SRC_SYSCALLS = $(wildcard $(SYSCALLSDIR)/*.c)
SYSCALLS_OBJECTS = $(patsubst $(SYSCALLSDIR)/%.c, $(OBJECT)/%.o,$(SRC_SYSCALLS))

$(SYSCALLS_OBJECTS): $(OBJECT)/%.o: $(SYSCALLSDIR)/%.c
	$(info in syscalls set)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

# userland/utils set 
UTILSDIR = $(USERLAND)/utils
SRC_UTILS = $(wildcard $(UTILSDIR)/*.c)
UTILS_OBJECTS = $(patsubst $(UTILSDIR)/%.c, $(OBJECT)/%.o,$(SRC_UTILS))

$(UTILS_OBJECTS): $(OBJECT)/%.o: $(UTILSDIR)/%.c 
	$(info in utils set)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

asst03.elf: $(LIB_OBJECTS) $(MAIN_OBJECTS) $(DEV_OBJECTS) $(DEV_OBJECTS) $(SYS_OBJECTS) $(CM4_OBJECTS) $(SYSCALLS_OBJECTS) $(UTILS_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $(TARGET)/$@

clean:
	rm -rf $(OBJECT)/*.o $(MAPDIR)/*.map $(TARGET)/* *.o *.elf

# To Students -- Modify to match with the appropriate driver file.
# load:
# 	openocd -f /usr/share/openocd/scripts/board/stm32f4discovery.cfg -f /usr/share/openocd/scripts/interface/stlink.cfg -c "program target/asst03 verify reset exit"

load:
	openocd -f board/stm32f4discovery.cfg -f interface/stlink.cfg -c "program target/asst03.elf verify reset exit"

load2: 
	openocd -f board/stm32f4discovery.cfg -f interface/stlink.cfg

debug:
	arm-none-eabi-gdb

