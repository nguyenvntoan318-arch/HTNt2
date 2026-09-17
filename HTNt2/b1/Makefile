TARGET = b1

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = cortex-m3

CFLAGS = -mcpu=$(CPU) -mthumb
CFLAGS += -Wall -Wextra
CFLAGS += -O0
CFLAGS += -g3
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections

CFLAGS += -DSTM32F103xB
CFLAGS += -DUSE_HAL_DRIVER

CFLAGS += -I.
CFLAGS += -IDrivers/STM32F1xx_HAL_Driver/Inc
CFLAGS += -IDrivers/CMSIS/Core/Include
CFLAGS += -IDrivers/CMSIS/Device/ST/STM32F1xx/Include

LDFLAGS = -mcpu=$(CPU) -mthumb
LDFLAGS += -T stm32f103.ld
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -specs=nano.specs
LDFLAGS += -specs=nosys.specs

HAL_SRC = \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c

CMSIS_SRC = \
Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c

SRC = main.c $(HAL_SRC) $(CMSIS_SRC)

OBJ = $(SRC:.c=.o)

all: $(TARGET).elf $(TARGET).bin
	$(SIZE) $(TARGET).elf

$(TARGET).elf: $(OBJ) startup_stm32f103.o
	$(CC) $(LDFLAGS) $^ -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

startup_stm32f103.o: startup_stm32f103.s
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -f startup_stm32f103.o
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin

.PHONY: all clean
