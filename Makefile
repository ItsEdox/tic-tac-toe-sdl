all: build

build:
	gcc -o sdl main.c game/minimax.c `pkg-config --cflags --libs sdl2` -lm
