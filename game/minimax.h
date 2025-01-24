extern char gameBoard[9];

int computerMove(char arr[]);
int playerMove(char arr[], int move);
int minimax(int depth, char arr[], bool isMax);
int checkForWin(char arr[], char player);
int countArray(char arr[]);
