# WS2812 output Makefile
include ../../common/avr_tool.mk
include ../../common/arduino_config.mk
DETECTUSB=../../common/detectusb.sh

all: rgbled_uno.elf rgbled_pro.elf

clean:
	$(RM) rgbled_*.hex rgbled_*.elf ws2812_*.[os] rgbled_test_*.[os]

flash-uno: rgbled_uno.hex
	avrdude -p atmega328p -c arduino -P `sh $(DETECTUSB)` -U flash:w:$^
flash-pro: rgbled_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

rgbled_uno.elf: rgbled_test_uno.o ws2812_uno.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

rgbled_pro.elf: rgbled_test_pro.o ws2812_pro.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

VPATH += ..

CFLAGS += -Os -Wall -Wstrict-prototypes
#CFLAGS +=  -v
CPPFLAGS += -I../../common -I.. -DTEST_DI_PIN=D,7 -DWS2812_DI_FREEPIN

ifeq ($(strip $(HELIX)),)
  # Pro Micro:    A0 pin
  #   ws2812_pro.o ws2812_pro.s: CPPFLAGS += -DWS2812_DI_PIN=F,7
  # Pro Micro:    10 pin
  ws2812_pro.o ws2812_pro.s: CPPFLAGS += -DWS2812_DI_PIN=B,6
  # Pro Micro:    6 pin
  # ws2812_pro.o ws2812_pro.s: CPPFLAGS += -DWS2812_DI_PIN=D,7
else
  # Helix:        TX pin
  ws2812_pro.o ws2812_pro.s: CPPFLAGS += -DWS2812_DI_PIN=D,3
  rgbled_test_pro.o: CPPFLAGS += -DHELIX
endif
# Arduino Uno R3: A0 pin
ws2812_uno.o ws2812_uno.s: CPPFLAGS += -DWS2812_DI_PIN=C,0

ifneq  ($(strip $(LED_NUMS)),)
  rgbled_test_pro.o rgbled_test_uno.o: CPPFLAGS += -DLED_NUMS=$(LED_NUMS)
endif
