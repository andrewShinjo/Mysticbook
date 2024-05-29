gcc $( pkg-config --cflags gtk4 ) -o main main.c $( pkg-config --libs gtk4 )
