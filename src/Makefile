CROSS	?= /opt/cross/avr/bin/avr-
CC	 = $(CROSS)gcc
OBJCOPY	 = $(CROSS)objcopy

MCU	= attiny85
F_CPU	= 8000000

TARGET	= main

CFLAGS	+= -DF_CPU=$(F_CPU)UL -mmcu=$(MCU)
CFLAGS	+= -std=c99 -Os -Wall -Wstrict-prototypes -Wa,-adhlns=$(<:.c=.lst)

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
	micronucleus --run $^

clean:
	$(RM) $(OBJS) $(TARGET).hex $(TARGET).elf $(TARGET).map *.lst img.h
