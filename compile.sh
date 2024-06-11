gcc $( pkg-config --cflags gtk4 ) -o main main.c text_block.c $( pkg-config --libs gtk4 ) -g
