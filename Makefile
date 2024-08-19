CC := gcc
CFLAGS := $(shell pkg-config --cflags gtk4)
LDFLAGS := $(shell pkg-config --libs gtk4) -lsqlite3
BUILDDIR := build/
SRCDIR := src
FRONTEND_DIR := $(SRCDIR)/frontend
BACKEND_DIR := $(SRCDIR)/backend
REPOSITORY_DIR := $(BACKEND_DIR)/repository
COMPONENT_DIR := $(FRONTEND_DIR)/components
PAGE_DIR := $(FRONTEND_DIR)/pages
FRONTEND := $(wildcard $(FRONTEND_DIR)/*.c)
COMPONENTS := $(wildcard $(COMPONENT_DIR)/*.c)
PAGES := $(wildcard $(PAGE_DIR)/*.c)
BACKEND := $(wildcard $(BACKEND_DIR)/*.c)
REPOSITORIES := $(wildcard $(REPOSITORY_DIR)/*.c)
SRCS := $(FRONTEND) $(COMPONENTS) $(PAGES) $(BACKEND) $(REPOSITORIES)
EXEC := main

all: run

$(EXEC): $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(BUILDDIR)$(EXEC) $(SRCS) $(LDFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

run: $(EXEC)
	./$(BUILDDIR)$(EXEC)

clean:
	rm -rf $(BUILDDIR) $(EXEC)
