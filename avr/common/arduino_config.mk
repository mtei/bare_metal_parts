# select Arduino UNO, Pro Micro

# Arduino UNO configure
%_uno.o %_uno.s: CPPFLAGS += -DF_CPU=16000000L
%_uno.o %_uno.s: CFLAGS  += -mmcu=atmega328p
%_uno.elf:       LDFLAGS += -mmcu=atmega328p
%_uno.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
%_uno.s: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -S $(OUTPUT_OPTION) $<

# Pro Micro configure
%_pro.o %_pro.s: CPPFLAGS += -DF_CPU=16000000L
%_pro.o %_pro.s: CFLAGS  += -mmcu=atmega32u4
%_pro.elf:       LDFLAGS += -mmcu=atmega32u4
%_pro.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
%_pro.s: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -S $(OUTPUT_OPTION) $<
