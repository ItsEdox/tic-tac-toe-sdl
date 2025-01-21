all: build

build:
	gcc -o sdl main.c minimax.c `pkg-config --cflags --libs sdl2` -lm
