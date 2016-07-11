DEPS = kernel
OUTPUT = corax
PHONY = $(DEPS) clean

all: $(OUTPUT)

$(OUTPUT): $(DEPS)
	mv kernel/corax $(OUTPUT)

$(DEPS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(DEPS)
	rm -f $(OUTPUT)

.PHONY: $(PHONY)
