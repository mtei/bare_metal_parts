# semi_soft_serial Test program Makefile
include ../../common/avr_tool.mk
include ../../common/arduino_config.mk
DETECTUSB=../../common/detectusb.sh

all: initiator_pro.elf responder_pro.elf trloop_pro.elf

clean:
	$(RM) initiator_*.hex semi_soft_serial_*.o  trloop_*.o

flash-i_uno: initiator_uno.hex
	avrdude -p atmega328p -c arduino -P `sh $(DETECTUSB)` -U flash:w:$^
flash-i_pro: initiator_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

flash-r_uno: responder_uno.hex
	avrdude -p atmega328p -c arduino -P `sh $(DETECTUSB)` -U flash:w:$^
flash-r_pro: responder_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

flash-tr_pro: trloop_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

initiator_uno.elf: initiator_uno.o semi_soft_serial_uno.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

initiator_pro.elf: initiator_pro.o semi_soft_serial_pro.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

responder_uno.elf: responder_uno.o semi_soft_serial_uno.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

responder_pro.elf: responder_pro.o semi_soft_serial_pro.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

trloop_pro.elf: trloop_pro.o

semi_soft_serial_uno.o: Makefile
semi_soft_serial_pro.o: Makefile

VPATH += ..

CFLAGS += -Os -Wall -Wstrict-prototypes
CPPFLAGS += -I../../common -I..
#CPPFLAGS += -DPARITY_ENABLE
CPPFLAGS += -DHDSS_DEBUG_PIN=B,5
#  CPPFLAGS += -DF_CPU=16000000L  in ../../common/arduino_config.mk
