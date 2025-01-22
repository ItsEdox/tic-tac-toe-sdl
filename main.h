#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#define WINDOW_TITLE "Tic-Tac-Toe"
#define WINDOW_WIDTH 750
#define WINDOW_HEIGHT 750
#define SQUARE_WH 150
#define SQUARE_XY 100
#define RADIUS 70

#define FPS 60

typedef struct {
    SDL_Window* win;
    SDL_Renderer* render;
    SDL_Event* event;
} window;

