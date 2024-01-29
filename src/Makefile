
EE_BIN = ./fs/main.elf
EE_OBJS = main.o memcard_test.o vu1/vu1.o vu1/vu1_test.o
EE_LIBS = -lcdvd -lmc -lpacket2 -ldma

.PHONY: all

all: $(EE_BIN) 
	mkisofs -o ../build/functests.iso ./fs

.PHONY: clean
clean:
	rm -f *.o $(EE_BIN) ../build/ps-functests.iso

include $(PS2SDK)/Defs.make
include ../make/Makefile.pref
include ../make/Makefile.eeglobal
