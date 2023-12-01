
EE_BIN = ./fs/main.elf
EE_OBJS = main.o 
EE_LIBS = -lcdvd

.PHONY: all

all: $(EE_BIN) 
	mkisofs -o ../build/cdread.iso ./fs

.PHONY: clean
clean:
	rm -f *.o $(EE_BIN) ../build/ps-functests.iso

include $(PS2SDK)/Defs.make
include ../make/Makefile.pref
include ../make/Makefile.eeglobal
