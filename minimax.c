#include <math.h>
#include <stdbool.h>
#include "main.h"
#include "minimax.h"

char gameBoard[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

int playerMove(char arr[], int move) {
    if (arr[move] == ' ') {
        arr[move] = 'X';
        return 0;
    }
    else {
        return 1;
    }
}

int countArray(char arr[]) {
    int j = 0;
    for (int i = 0; i < 9; i++) {
        if (arr[i] != ' ') {
            j++;
        }
    }
    return j;
}

int checkForWin(char arr[], char player) {
    /*
    [0, 1, 2], [3, 4, 5], [6, 7, 8],
    [0, 3, 6], [1, 4, 7], [2, 5, 8],
    [0, 4, 8], [2, 4, 6]
    These are winning moves
    */
    if ((arr[0] == player && arr[1] == player && arr[2] == player) ||
        (arr[3] == player && arr[4] == player && arr[5] == player) ||
        (arr[6] == player && arr[7] == player && arr[8] == player) ||
        (arr[0] == player && arr[3] == player && arr[6] == player) ||
        (arr[1] == player && arr[4] == player && arr[7] == player) ||
        (arr[2] == player && arr[5] == player && arr[8] == player) ||
        (arr[0] == player && arr[4] == player && arr[8] == player) ||
        (arr[2] == player && arr[4] == player && arr[6] == player)) {
            return 2;
        } 
    else if (countArray(arr) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int minimax(int depth, char arr[], bool isMax) {
    int score;
    
    if (checkForWin(arr, 'X') == 2) {
        return -1;
    }
    if (checkForWin(arr, 'O') == 2) {
        return 1;
    }
    if (checkForWin(arr, 'O') == 1) {
        return 0;
    }

    if (isMax) {
        int best_score = -10000;
        for (int i = 0; i < 9; i++) {
            if (arr[i] == ' ') {
                arr[i] = 'O';
                score = minimax(depth + 1, arr, false);
                arr[i] = ' ';
                best_score = fmax(score, best_score);
            }
        }
        return best_score;
    }
    else {
        int best_score = 10000;
        for (int i = 0; i < 9; i++) {
            if (arr[i] == ' ') {
                arr[i] = 'X';
                score = minimax(depth + 1, arr, true);
                arr[i] = ' ';
                best_score = fmin(score, best_score);
            }
        }
        return best_score;
    }
}

int computerMove(char arr[]) {
    int best_score = -10000;
    int best_move;
    int score;
    for (int i = 0; i < 9; i++) {
        if (arr[i] == ' ') {
            arr[i] = 'O';
            score = minimax(0, arr, false);
            arr[i] = ' ';
            if (score > best_score) {
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}

