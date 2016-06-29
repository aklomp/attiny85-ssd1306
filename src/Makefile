BINDIR	= /opt/cross/avr/bin
CC	= $(BINDIR)/avr-gcc
LD	= $(BINDIR)/avr-ld
OBJCOPY	= $(BINDIR)/avr-objcopy
OBJDUMP	= $(BINDIR)/avr-objdump
AVRDUDE	= avrdude

MCU	= attiny85
F_CPU	= 8000000

TARGET	= main

CFLAGS	= -Os -DF_CPU=$(F_CPU)UL -mmcu=$(MCU) -Wall -std=gnu99 \
	  -Wall -Wstrict-prototypes -Wa,-adhlns=$(<:.c=.lst)

LDFLAGS	= -Wl,-Map=$(TARGET).map,--cref

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

.PHONY: clean flash

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

ssd1306.c: img.h

img.h: img.png
	python convert.py $^ > $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $^

flash: $(TARGET).hex
	micronucleus --run $(TARGET).hex

clean:
	$(RM) $(OBJS) $(TARGET).hex $(TARGET).elf $(TARGET).map *.lst img.h
