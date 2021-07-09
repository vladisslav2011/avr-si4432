MCU=atmega8
CFLAGS+=-Os -Wall  -mmcu=$(MCU) -DF_CPU=16000000
CFLAGS+=-funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS+=-Wl,-relax -ffreestanding -fomit-frame-pointer 
CFLAGS+=-fno-tree-scev-cprop -fno-strict-aliasing 
#CFLAGS+=-fwhole-program
CFLAGS+= -Wl,-gc-sections -ffunction-sections -fdata-sections -mcall-prologues
CFLAGS+=-fno-move-loop-invariants -fno-inline-small-functions
CFLAGS+=-Iusbdrv -I.
CC=avr-gcc
LD=avr-ld


OBJECTS=usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o 

NAME=si4432-usb

AVRUSBBOOT=../../../avrusbboot.2006-06-25/software/avrusbboot



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



all: clean build
clean:
	rm -f $(OBJECTS) $(NAME).hex $(NAME).bin $(NAME).elf $(NAME).o
build: $(NAME).bin $(NAME).s
boot: $(NAME).bin
	ls -l $(NAME).bin
	$(AVRUSBBOOT) wb $(NAME).bin
	$(AVRUSBBOOT) -r

$(NAME).elf: $(NAME).o $(OBJECTS)





