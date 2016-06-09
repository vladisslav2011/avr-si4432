MCU=atmega8
CFLAGS+=-Os -Wall  -mmcu=$(MCU) -DF_CPU=8000000
CFLAGS+=-funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS+=-Wl,-relax -ffreestanding -fomit-frame-pointer 
CFLAGS+=-fno-tree-scev-cprop -fno-strict-aliasing 
CFLAGS+=-fwhole-program
CFLAGS+= -Wl,-gc-sections -ffunction-sections -fdata-sections -mcall-prologues
CFLAGS+=-fno-move-loop-invariants -fno-inline-small-functions
CFLAGS+=-Iusbdrv -I.
CC=avr-gcc
LD=avr-ld


OBJECTS=

DEPS=../si443x_regs.h softI2C.h st7558.h font.h

NAME=si4432-lt

WR=../../usbtest/from-work/commandline/test
#WR=../../usbtest/commandline/test



%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $< $(OBJECTS)
%.bin: %.elf
	avr-objcopy -j .text -O binary $< $@
%.hex: %.elf
	avr-objcopy -j .text -O ihex $< $@
.c.s:
	$(CC) $(CFLAGS) -S -fverbose-asm -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
.S.o:
	$(CC) $(CFLAGS) -c -x assembler-with-cpp -o $@ $<
.c.elf:
	$(CC) $(CFLAGS) -o $@ $<



all: build
clean:
	rm -f *.o *.elf *.s $(NAME).hex $(NAME).bin $(NAME).elf $(NAME).o
build: $(NAME).bin $(NAME).s
boot: $(NAME).bin
	ls -l $(NAME).bin
	$(WR) --spiclk 100 atmega8 erase atmega8 write_flash $(NAME).bin atmega8 verify_flash $(NAME).bin atmega8 run

$(NAME).elf: $(NAME).c $(DEPS)





