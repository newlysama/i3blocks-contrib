CC      ?= clang
CFLAGS  ?= -O2 -s
MODULES := disk_usage date memory cpu

# List binaries to be compiled
BINS := $(foreach m,$(MODULES),$(m)/$(m))

.PHONY: all clean $(MODULES)

all: $(BINS)

clean:
	rm -f $(BINS)
