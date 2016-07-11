OBJECTS = main.o
OUTPUT = core.o
INCLUDES = -I../include -I../../include -I../..
CFLAGS += $(INCLUDES)
PHONY = clean

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	ld -r $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.PHONY: $(PHONY)
