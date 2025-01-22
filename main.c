#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "minimax.h"

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>

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

static void drawCross(window* w, int arr[], int v) {
    SDL_SetRenderDrawColor(w->render, 0, 0, 0, 255);
    SDL_RenderDrawLine(w->render, arr[v], arr[v + 1], arr[v] + SQUARE_WH, arr[v + 1] + SQUARE_WH);
    SDL_RenderDrawLine(w->render, arr[v] + SQUARE_WH, arr[v  + 1], arr[v], arr[v + 1] + SQUARE_WH);
    SDL_RenderPresent(w->render);
}

static void drawCircle(window* w, int board[], int cord, int radius) {
    SDL_SetRenderDrawColor(w->render, 0, 0, 0, 255);
    int centreX = board[cord] + (SQUARE_WH / 2);
    int centreY = board[cord + 1] + (SQUARE_WH / 2);
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
    for (int i = 0; i < 18; i++) {
      if (arr[i] <= mx && mx <= (arr[i] + SQUARE_WH)) {
	if (arr[i + 1] <= my && my <= (arr[i + 1] + SQUARE_WH)) {
	  /* Debug Code

	   printf("%d\n", i);
	   printf("%d\n", arr[i]);
	   printf("%d\n", arr[i + 1]);
	  */
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
    return -1;
}

static void initRender(window* w) {
    w->render = SDL_CreateRenderer(w->win, -1, 0);
    SDL_RenderSetVSync(w->render, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
}

void setup(window* w) {
    bool programRunning = true, playersMove = false;
    initRender(w);
    SDL_Point mouse;
    int mx, my, board[18], i;
    while (programRunning) {
        drawGrid(w, board);
	if (playersMove == false) {
	  gameBoard[computerMove(gameBoard)] = 'O';
	  // printf("%d\n", checkForWin(gameBoard, 'O'));
	  if (checkForWin(gameBoard, 'O') == 1) {
	     printf("O Wins!");
	     programRunning = false;
	  }
	  if (countArray(gameBoard) == 9) {
	     printf("Tie!");
	     programRunning = false;
	   }
	   playersMove = true;
	}
	for (int g = 0; g < 9; g++) {
	  if (gameBoard[g] == 'X') {
	     drawCross(w, board, g*2);  
	  }
	  if (gameBoard[g] == 'O') {
	     drawCircle(w, board, g*2, RADIUS);
	  }
	}
        while (SDL_PollEvent(w->event)) {
	  switch (w->event->type) {
	     case SDL_QUIT:
	        programRunning = false;
	        break;
	     case SDL_MOUSEBUTTONDOWN:
	        SDL_GetMouseState(&mx, &my);
		break;
	     case SDL_MOUSEBUTTONUP:
	        i = check_for_rect_click(board, mx, my);
		if (i != -1) {
		  if (playerMove(gameBoard, i/2) == 1) {
		    //debug, change later!
		    printf("Invaild Move!");
		  }
		  else {
		    if (checkForWin(gameBoard, 'X') == 1) {
		      printf("X Wins!");
		      programRunning = false;
		    }
		    playersMove = false;
		  }
		}
		break;
	     default:
	        break;
	  }
	}
    }
}
int main(void) {
    window* w;
    if ((SDL_INIT_VIDEO) == -1) {
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
