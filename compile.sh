gcc $( pkg-config --cflags gtk4 ) -o main main.c text_block.c util.c $( pkg-config --libs gtk4 )
