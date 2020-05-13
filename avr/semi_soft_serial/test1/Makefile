# semi_soft_serial Test program 1 Makefile
include ../../common/avr_tool.mk
include ../../common/arduino_config.mk
DETECTUSB=../../common/detectusb.sh

help:
	@echo "semi_soft_serial Test program 1: initiator send 2 byte test"
	@echo " $ make clean flash-i_pro  # flash write initiator"
	@echo " $ make clean flash-r_pro  # flash write responder"
	@echo "  BUFFER_SIZE=8  # interrupt receive buffer size"

all: initiator_pro.elf responder_pro.elf

clean:
	$(RM) initiator_*.o responder_*.o semi_soft_serial_*.o
	$(RM) initiator_*.elf responder_*.elf
	$(RM) initiator_*.hex responder_*.hex
	$(RM) debug_serial_*.o

flash-i_uno: initiator_uno.hex
	avrdude -p atmega328p -c arduino -P `sh $(DETECTUSB)` -U flash:w:$^
flash-i_pro: initiator_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

flash-r_uno: responder_uno.hex
	avrdude -p atmega328p -c arduino -P `sh $(DETECTUSB)` -U flash:w:$^
flash-r_pro: responder_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

initiator_uno.elf: initiator_uno.o semi_soft_serial_uno.o debug_serial_uno.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

initiator_pro.elf: initiator_pro.o semi_soft_serial_pro.o debug_serial_pro.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

responder_uno.elf: responder_uno.o semi_soft_serial_uno.o debug_serial_uno.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

responder_pro.elf: responder_pro.o semi_soft_serial_pro.o debug_serial_pro.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

semi_soft_serial_uno.o: Makefile
semi_soft_serial_pro.o: Makefile

VPATH += ..

CFLAGS += -Os -Wall -Wstrict-prototypes
CPPFLAGS += -I../../common -I..
#CPPFLAGS += -DPARITY_ENABLE
CPPFLAGS += -DHDSS_DEBUG_PIN=B,5
#  CPPFLAGS += -DF_CPU=16000000L  in ../../common/arduino_config.mk
ifneq ($(strip $(BUFFER_SIZE)),)
  CPPFLAGS += -DHDSS_RECEIVE_BUFFER_SIZE=$(BUFFER_SIZE)
else
  CPPFLAGS += -DHDSS_RECEIVE_BUFFER_SIZE=8
endif
