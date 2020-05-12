# semi_soft_serial Test program 2 Makefile
include ../../common/avr_tool.mk
include ../../common/arduino_config.mk
DETECTUSB=../../common/detectusb.sh

help:
	@echo "semi_soft_serial Test program 2"
	@echo " $ make [options] clean flash-i_pro  # flash write initiator"
	@echo " $ make [options] clean flash-r_pro  # flash write responder"
	@echo "options:"
	@echo "  BUFFER_SIZE=8  # interrupt receive buffer size"
	@echo "  BULK=4         # responder bulk read chunk size"
	@echo "  DELTA=4        # initiator send BUFFER_SIZE+DELTA bytes"
	@echo "  SPACE=0        # initiator send bytes inter gap"

all: initiator_pro.elf responder_pro.elf

clean:
	$(RM) initiator_*.hex initiator_*.elf initiator_*.o
	$(RM) responder_*.hex responder_*.elf responder_*.o
	$(RM) semi_soft_serial_*.o debug_serial_*.o

flash-i_pro: initiator_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

flash-r_pro: responder_pro.hex
	avrdude -p atmega32u4 -c avr109  -P `sh $(DETECTUSB)` -U flash:w:$^

initiator_pro.elf: initiator_pro.o semi_soft_serial_pro.o debug_serial_pro.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

responder_pro.elf: responder_pro.o semi_soft_serial_pro.o debug_serial_pro.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

semi_soft_serial_pro.o: Makefile

VPATH += ..

CFLAGS += -Os -fno-aggressive-loop-optimizations -Wall -Wstrict-prototypes
CPPFLAGS += -I../../common -I..
#CPPFLAGS += -DPARITY_ENABLE
CPPFLAGS += -DHDSS_DEBUG_PIN=B,5
ifneq ($(strip $(BUFFER_SIZE)),)
  CPPFLAGS += -DHDSS_RECEIVE_BUFFER_SIZE=$(BUFFER_SIZE)
else
  CPPFLAGS += -DHDSS_RECEIVE_BUFFER_SIZE=8
endif
ifneq ($(strip $(BULK)),)
  CPPFLAGS += -DBULK=$(BULK)
endif
ifneq ($(strip $(DELTA)),)
  CPPFLAGS += -DDELTA=$(DELTA)
endif
ifneq ($(strip $(SPACE)),)
  CPPFLAGS += -DSPACE=$(SPACE)
endif

# CPPFLAGS += -DF_CPU=16000000L  in ../../common/arduino_config.mk
