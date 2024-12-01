CC := gcc
CFLAGS := $(shell pkg-config --cflags gtk4) -g
LDFLAGS := $(shell pkg-config --libs gtk4)
BUILDDIR := build/
SRCDIR := src
COMPONENT_DIR := $(SRCDIR)/components
MODEL_DIR := $(SRCDIR)/model
COMPONENTS := $(wildcard $(COMPONENT_DIR)/*.c)
MODELS := $(wildcard $(MODEL_DIR)/*.c)
SRCS := $(COMPONENTS) $(MODELS) $(wildcard $(SRCDIR)/*.c) 
MAIN := main

all: run

$(MAIN): $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(BUILDDIR)$(MAIN) $(SRCS) $(LDFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

run: $(MAIN)
	./$(BUILDDIR)$(MAIN)

clean:
	rm -rf $(BUILDDIR) $(MAIN)
