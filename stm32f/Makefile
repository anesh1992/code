#
# Makefile for simple monitor on STM32F3 Discovery Board
#
# openocd -f board/stm32f3discovery.cfg -c "init" -c "halt" -c "flash write_image erase simple_monitor.elf" -c "reset run" -c shutdown


# C source files for the project
PROJ_NAME = simple_monitor
SRCS = main.c 

# Simple Monitor sources
SRCS += monitor.c uart.c printf_stdarg.c parser.c dump.c syscall.c terminal.c \
	decoder/decoder.c decoder/STM32F30x_decoder.c 


###################################################
# Location of the linker scripts
LDSCRIPT_INC=ld

# Location of CMSIS files for our device
CMSIS     = Drivers/CMSIS
CMSIS_INC = $(CMSIS)/Include
CMSIS_DEV = $(CMSIS)/Device/ST/STM32F3xx
CMSIS_DEV_INC = $(CMSIS_DEV)/Include
CMSIS_DEV_SRC = $(CMSIS_DEV)/Source/Templates
SRCS += $(CMSIS_DEV_SRC)/system_stm32f3xx.c

# Location of HAL drivers
HAL     = Drivers/STM32F3xx_HAL_Driver
HAL_INC = $(HAL)/Inc
HAL_SRC = $(HAL)/Src
SRCS   += $(HAL_SRC)/stm32f3xx_hal_rcc.c \
		  $(HAL_SRC)/stm32f3xx_hal_rcc_ex.c \
          $(HAL_SRC)/stm32f3xx_hal.c \
          $(HAL_SRC)/stm32f3xx_hal_cortex.c \
          $(HAL_SRC)/stm32f3xx_hal_uart.c \
          $(HAL_SRC)/stm32f3xx_hal_gpio.c \
          $(HAL_SRC)/stm32f3xx_hal_pcd.c \
          $(HAL_SRC)/stm32f3xx_hal_pcd_ex.c \
		  $(HAL_SRC)/stm32f3xx_hal_i2c.c \
		  $(HAL_SRC)/stm32f3xx_hal_spi.c \
		  $(HAL_SRC)/stm32f3xx_hal_adc.c \
		  $(HAL_SRC)/stm32f3xx_hal_adc_ex.c \
		  $(HAL_SRC)/stm32f3xx_hal_tim.c \
		  $(HAL_SRC)/stm32f3xx_hal_tim_ex.c

# USB Sources
USB_CORE     = Drivers/STM32_USB_Device_Library/Core
USB_CORE_INC = $(USB_CORE)/Inc
USB_CORE_SRC = $(USB_CORE)/Src
SRCS   += $(USB_CORE_SRC)/usbd_core.c \
          $(USB_CORE_SRC)/usbd_ctlreq.c \
          $(USB_CORE_SRC)/usbd_ioreq.c

USB_CLASS     = Drivers/STM32_USB_Device_Library/Class/CDC
USB_CLASS_INC = $(USB_CLASS)/Inc
USB_CLASS_SRC = $(USB_CLASS)/Src
SRCS   += $(USB_CLASS_SRC)/usbd_cdc.c \
          usbd_conf.c usbd_desc.c usbd_cdc_interface.c

# Location of BSP Files
BSP     = Drivers/BSP/STM32F3-Discovery
BSP_INC = $(BSP)
BSP_SRC = $(BSP)
SRCS   += $(BSP)/stm32f3_discovery.c \
	  $(BSP)/stm32f3_discovery_accelerometer.c \
	  $(BSP)/stm32f3_discovery_gyroscope.c

# Location of Component Files
CMP     = Drivers/BSP/Components
CMP_INC = $(CMP)
CMP_SRC = $(CMP)
SRCS   += $(CMP)/lsm303dlhc/lsm303dlhc.c \
	  $(CMP)/l3gd20/l3gd20.c

#Location of FreeRtos Files
FREERTOS = FreeRTOS
FREERTOS_INC = $(FREERTOS)/include
FREERTOS_SRC = $(FREERTOS)
SRCS += $(FREERTOS)/croutine.c \
		$(FREERTOS)/event_groups.c \
		$(FREERTOS)/list.c \
		$(FREERTOS)/queue.c \
		$(FREERTOS)/tasks.c \
		$(FREERTOS)/timers.c
FREERTOS_PORTABLE = $(FREERTOS)/portable
FREERTOS_PORTABLE_GCC_INC = $(FREERTOS_PORTABLE)/GCC/ARM_CM4F
FREERTOS_PORTABLE_GCC_SRC = $(FREERTOS_PORTABLE)/GCC/ARM_CM4F
SRCS += $(FREERTOS_PORTABLE_GCC_SRC)/port.c
FREERTOS_PORTABLE_MemMang_SRC= $(FREERTOS_PORTABLE)/MemMang
SRCS += $(FREERTOS_PORTABLE_MemMang_SRC)/heap_4.c

PREFIX	=	arm-none-eabi-
CC=$(PREFIX)gcc
AR=$(PREFIX)ar
AS=$(PREFIX)as
GDB=$(PREFIX)gdb
OBJCOPY=$(PREFIX)objcopy
OBJDUMP=$(PREFIX)objdump
SIZE=$(PREFIX)size

#https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
#CFLAGS  = -Wall -g -std=c99 -Os 
#-Wall : all warning	-g: debugging in operating system format	-std: C standard 
CFLAGS  = -Wall -g -std=gnu99
# -Os: Optimize for size.
CFLAGS += -Os  
# -Werror: Treat all warning as error and not compile
CFLAGS += -Werror
#Processor options	https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html#ARM-Options
#	-mlittle-endian: Generate code for a processor running in little-endian mode.
#	-mcpu: This specifies the name of the target ARM processor.
#	-march: This specifies the name of the target ARM architecture.
CFLAGS += -mlittle-endian -mcpu=cortex-m4  -march=armv7e-m 
# -mthumb: mthumb instruction set not arm
CFLAGS += -mthumb
#CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
# mfpu: This specifies what floating-point hardware (or hardware emulation) is available on the target.
# -mfloat-abi: Specifies which floating-point ABI to use.
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
# -ffunction-sections -fdata-sections: Place each function or data item into its own section in the output file if the target supports arbitrary sections.
CFLAGS += -ffunction-sections -fdata-sections
# -I: These options specify directories to search for header files, for libraries and for parts of the compiler
CFLAGS += -I .
CFLAGS += -I $(CMSIS_INC)
CFLAGS += -I $(CMSIS_DEV_INC)
CFLAGS += -DSTM32F303xC
CFLAGS += -I $(HAL_INC)
CFLAGS += -I $(BSP_INC)
CFLAGS += -I $(USB_CORE_INC)
CFLAGS += -I $(USB_CLASS_INC)
CFLAGS += -I $(FREERTOS_INC)
CFLAGS += -I $(FREERTOS_PORTABLE_GCC_INC)

LDFLAGS  = -Wall -g -std=c99 -Os  
LDFLAGS += -mlittle-endian -mcpu=cortex-m4  -march=armv7e-m 
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map
LDFLAGS += -mthumb
#LDFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
LDFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LDFLAGS += -ffunction-sections -fdata-sections

ASFLAGS =  -g -mlittle-endian -mcpu=cortex-m4  -march=armv7e-m
ASFLAGS += -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
###################################################

STARTUP = $(CMSIS_DEV_SRC)/gcc/startup_stm32f303xc.o # add startup file to build

OBJS = $(addsuffix .o,$(basename $(SRCS)))
#substitution reference $(SRCS:.c=.d)
DEPS = $(addprefix deps/,$(SRCS:.c=.d)) 

###################################################

.PHONY: all proj program debug clean reallyclean 

all: proj

-include $(DEPS)

proj: 	$(PROJ_NAME).elf

deps:
	@mkdir -p $@

%.o : %.c deps
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

$(PROJ_NAME).elf: $(OBJS) $(STARTUP)
	$(CC) $(LDFLAGS) $^ -o $@ -L$(STD_PERIPH_LIB) -lstm32f3 -L$(LDSCRIPT_INC) -Tsimple_monitor.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf

program: all
	openocd -f board/stm32f3discovery.cfg -c "init" -c "reset init" -c "halt" -c "flash write_image erase $(PROJ_NAME).elf" -c "reset run" -c shutdown

debug: program
	$(GDB) --tui -x gdb/gdb_cmds $(PROJ_NAME).elf

clean:
	find ./ -name '*~' | xargs rm -f
	find ./ -name '*.o' | xargs rm -f
	rm -f deps/*.d
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst
	rm -f openocd.log
	-rmdir deps
