OBJECTS = arch/arch.o core/core.o
DEPS = arch core
LDFLAGS = --oformat=elf32-i386 -b elf32-i386 -m elf_i386
CFLAGS = -m32 -Wall -nostdlib -nodefaultlibs -nostartfiles -ffreestanding
PHONY = $(DEPS) clean
OUTPUT = corax

export LDFLAGS
export CFLAGS

all: $(OUTPUT)

$(DEPS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(OUTPUT): $(DEPS)
	ld -o $@ -T linker.ld $(LDFLAGS) $(OBJECTS)

clean: $(DEPS)
	rm -f $(OUTPUT)

.PHONY: $(PHONY)
