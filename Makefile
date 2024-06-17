CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

main: main.c text_block.c util.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
