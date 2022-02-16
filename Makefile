OPENOCD           ?= openocd
OPENOCD_INTERFACE ?= interface/stlink-v2.cfg
REV								?= D
PYTHON           ?= python3

PROG=lps-node-bootloader

HAL_ROOT=src/lib/stm32f0xx
CPU=f0
PROCESSOR=-mthumb -mcpu=cortex-m0 -DHSI48_VALUE="((uint32_t)48000000)" -DSTM32F072xB
OPENOCD_TARGET    ?= target/stm32f0x_stlink.cfg

INCLUDES=-Isrc/inc -Isrc/inc/$(CPU) -I$(HAL_ROOT)/Inc

# Platform specific files
OBJS+=src/src/startup_stm32f072xb.o src/src/system_stm32f0xx.o
OBJS+=src/src/gpio.o src/src/led.o src/src/bootmode.o src/src/system.o src/src/stm32f0xx_it.o
OBJS+=src/src/bootmanager.o

OBJS+=src/src/main.o

HALS+=gpio rcc cortex rcc_ex pwr
OBJS+=$(foreach mod, $(HALS), $(HAL_ROOT)/Src/stm32$(CPU)xx_hal_$(mod).o)
OBJS+=$(HAL_ROOT)/Src/stm32$(CPU)xx_hal.o

CFLAGS+=$(PROCESSOR) $(INCLUDES) -Os -g3 -Wall -Wno-pointer-sign -std=gnu11
LDFLAGS+=$(PROCESSOR) --specs=nano.specs --specs=nosys.specs -Ttools/make/stm32f072.ld -lm -lc

# Remove un-used functions and global variables from output file
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,-Map=bin/$(PROG).map,--cref,--gc-sections

PREFIX=arm-none-eabi-

CC=$(PREFIX)gcc
LD=$(PREFIX)gcc
AS=$(PREFIX)as
OBJCOPY=$(PREFIX)objcopy

all: check_submodules bin/$(PROG).elf bin/$(PROG).dfu

bin/$(PROG).elf: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)
	arm-none-eabi-size $@

clean:
	rm -f bin/$(PROG).elf bin/$(PROG).dfu bin/$(PROG).map $(OBJS)

flash:
	$(OPENOCD) -d2 -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c targets -c "reset halt" \
	           -c "flash write_image erase bin/lps-node-bootloader.elf" -c "verify_image bin/lps-node-bootloader.elf" -c "reset run" -c shutdown

openocd:
	$(OPENOCD) -d2 -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c targets

dfu:
	dfu-util -d 0483:df11 -a 0 -D bin/$(PROG).dfu -R

# Generic rules
%.bin: %.elf
	$(OBJCOPY) $^ -O binary $@

%.dfu: %.bin
	$(PYTHON2) tools/make/dfu-convert.py -b 0x8000000:$^ $@

check_submodules:
	$(PYTHON2) tools/make/check-for-submodules.py
