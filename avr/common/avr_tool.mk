# Makefile default command macros switch to AVR
CC = avr-gcc
LD = avr-ld
AR = avr-ar
AS = avr-as
CXX = avr-g++
OBJCOPY = avr-objcopy
HEXFORMAT = ihex
HEXGEN = $(OBJCOPY) -O $(HEXFORMAT) -R .eeprom -R .fuse -R .lock -R .signature

%.elf: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

%.hex: %.elf
	$(HEXGEN) $^ $@

%.s: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -S $(OUTPUT_OPTION) $<

ifneq ($(LIST),)
  CFLAGS += -Wa,-adhlns=$*.lst,-gstabs,--listing-cont-lines=100
endif

ifneq ($(MAP),)
  LDFLAGS += -Wl,-Map=$*.map,--cref
endif
