#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include "main.h"

static void drawGrid(window* w, int arr[]) {
    SDL_Rect square;
    square.h = SQUARE_WH; square.w = SQUARE_WH; square.x = SQUARE_XY; square.y = SQUARE_XY;
    // loop 18 times so x and y values can be stored in the array without being overwitten | 18 / 2 = 9
    for (int i = 0; i < 18; i++) {
        if (square.x > 500) {
            square.y = square.y + 200;
            square.x = 100;
        }
        SDL_SetRenderDrawColor(w->render, 255, 255, 255, 255);
        SDL_RenderFillRect(w->render, &square);
	arr[i] = square.x;
	arr[i + 1] = square.y;
	i++;
        square.x = square.x + 200;
    }
    SDL_RenderPresent(w->render);
}

static void drawCross(window* w) {
    SDL_SetRenderDrawColor(w->render, 0, 0, 0, 255);
    SDL_RenderDrawLine(w->render, 100, 100, 250, 250);
    SDL_RenderDrawLine(w->render, 250, 100, 100, 250);
    SDL_RenderPresent(w->render);
    SDL_Delay(1000 / FPS);
}

static void drawCircle(window* w, int radius) {
    SDL_SetRenderDrawColor(w->render, 0, 0, 0, 255);
    int centreX = 300 + (SQUARE_WH / 2);
    int centreY = 100 + (SQUARE_WH / 2);
    // Using Midpoint Circle Algorithm here
    int d = radius << 1; // Bitshifts radius left once so it doubles the radius, which equals d
    int x = radius - 1, y = 0, dx = 0, dy = 0;
    int error = dx - d;
    while (x >= y) {
       SDL_RenderDrawPoint(w->render, centreX + x, centreY - y);
       SDL_RenderDrawPoint(w->render, centreX + x, centreY + y);
       SDL_RenderDrawPoint(w->render, centreX - x, centreY - y);
       SDL_RenderDrawPoint(w->render, centreX - x, centreY + y);
       SDL_RenderDrawPoint(w->render, centreX + y, centreY - x);
       SDL_RenderDrawPoint(w->render, centreX + y, centreY + x);
       SDL_RenderDrawPoint(w->render, centreX - y, centreY - x);
       SDL_RenderDrawPoint(w->render, centreX - y, centreY + x);
       if (error <= 0) {
	  y++;
	  error = error + dy;
	  dy = dy + 2;
       }
       if (error > 0) {
	  x--;
	  dx = dx + 2;
	  error = error + dx - d; 
       }
    }
    SDL_RenderPresent(w->render);
}

static int check_for_rect_click(int arr[], int mx, int my) {
    printf("mx:%d, my:%d\n", mx, my);
    for (int i = 0; i < 18; i++) {
      if (arr[i] >= mx && arr[i] <= (arr[i] + SQUARE_WH)) {
	if (arr[i + 1] >= my && arr[i + 1] <= (arr[i + 1] + SQUARE_WH)) { // Something wrong with this!
	   i++;
	   return i;
	}
	else {
	   i++;
	}
      }
      else {
	 i++;
      }
    }
    return 0;
}

static void initRender(window* w) {
    w->render = SDL_CreateRenderer(w->win, -1, 0);
}

void setup(window* w) {
    bool programRunning = true;
    initRender(w);
    SDL_Point mouse;
    int mx, my, board[18];
    while (programRunning) {
        drawGrid(w, board);
	drawCircle(w, RADIUS);
        while (SDL_PollEvent(w->event)) {
	  switch (w->event->type) {
	     case SDL_QUIT:
	        programRunning = false;
	        break;
	     case SDL_MOUSEBUTTONDOWN:
	        SDL_GetMouseState(&mx, &my);
		int i = check_for_rect_click(board, mx, my);
		if (i != 0) {
		   drawCross(w);
		}
	     default:
	        break;
	  }
	  SDL_SetRenderDrawColor(w->render, 0, 0, 0, 255);
	}
    }
}
int main(void) {
    window* w;
    if ((SDL_INIT_EVERYTHING) == -1) {
        printf("Failed to start SDL. Error Code %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    atexit(SDL_Quit);

    w->win = SDL_CreateWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    setup(w);
    SDL_DestroyWindow(w->win);
    SDL_DestroyRenderer(w->render);
    
    SDL_Quit();
    return EXIT_SUCCESS;
}
