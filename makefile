all: vpet

vpet: vpet.o main.o sprites.o
	gcc -o vpet vpet.o main.o sprites.o `sdl2-config --libs`

vpet.o: vpet.c
	gcc -c `sdl2-config --cflags` vpet.c
main.o: main.c
	gcc -c `sdl2-config --cflags` main.c
sprites.o: sprites.c
	gcc -c `sdl2-config --cflags` sprites.c
