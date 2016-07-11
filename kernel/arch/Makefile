OBJECTS = boot.o debug.o init.o cpu.o cpu32.o entry.o interrupt.o paging.o apic.o i8259.o
OUTPUT = arch.o
LDFLAGS += -r
INCLUDES = -I../../include -I../include -I../..
CFLAGS += $(INCLUDES)
PHONY = clean

ifeq (0,$(MAKELEVEL))
LDFLAGS = --oformat=elf32-i386 -b elf32-i386 -m elf_i386 -r
CFLAGS = -m32 -Wall -nostdlib -nodefaultlibs -nostartfiles -ffreestanding $(INCLUDES)
endif
ASFLAGS = $(CFLAGS)

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	ld $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) $(OUTPUT)


.PHONY: $(PHONY)
