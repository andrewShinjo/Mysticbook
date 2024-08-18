CC := gcc
CFLAGS := $(shell pkg-config --cflags gtk4)
LDFLAGS := $(shell pkg-config --libs gtk4) -lsqlite3
BUILDDIR := build/
SRCDIR := src/
FRONTEND_DIR := $(SRCDIR)/frontend
BACKEND_DIR := $(SRCDIR)/backend
COMPONENT_DIR := $(FRONTEND_DIR)/components
PAGE_DIR := $(FRONTEND_DIR)/pages
SRCS := $(wildcard $(FRONTEND_DIR)/*.c)
COMPONENTS := $(wildcard $(COMPONENT_DIR)/*.c)
BACKEND := $(wildcard $(BACKEND_DIR)/*.c)
PAGES := $(wildcard $(PAGE_DIR)/*.c)
EXEC := main

all: run

$(EXEC): $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(BUILDDIR)$(EXEC) $(SRCS) $(COMPONENTS) $(PAGES) $(BACKEND) $(LDFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

run: $(EXEC)
	./$(BUILDDIR)$(EXEC)

clean:
	rm -rf $(BUILDDIR) $(EXEC)
