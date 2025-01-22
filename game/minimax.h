extern char gameBoard[9];

extern int computerMove(char arr[]);
extern int playerMove(char arr[], int move);
extern int minimax(int depth, char arr[], bool isMax);
extern int checkForWin(char arr[], char player);
extern int countArray(char arr[]);
