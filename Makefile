CC := gcc
CFLAGS := $(shell pkg-config --cflags gtk4)
LDFLAGS := $(shell pkg-config --libs gtk4)
BUILDDIR := build/
SRCDIR := src/frontend
COMPONENT_DIR := $(SRCDIR)/components
SRCS := $(wildcard $(SRCDIR)/*.c)
COMPONENTS := $(wildcard $(COMPONENT_DIR)/*.c)
EXEC := main

all: run

$(EXEC): $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(BUILDDIR)$(EXEC) $(SRCS) $(COMPONENTS) $(LDFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

run: $(EXEC)
	./$(BUILDDIR)$(EXEC)

clean:
	rm -rf $(BUILDDIR) $(EXEC)
