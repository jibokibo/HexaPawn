
//ohne Alpha-Beta Pruning
#ifndef HEXAPAWN_H
#define HEXAPAWN_H

#include <stdbool.h>
#include <time.h>

// global variables
extern char board[3][3];
extern const char HUMAN;
extern const char COMPUTER;
extern double cpuTimeUsed;
extern int nodeCount;
extern clock_t start;
extern clock_t end;

// function prototypes
int columnToIndex(char column);
int rowToIndex(int row);
void resetBoard();
void printBoard();
bool checkDestination(int end_row, int end_column);
int checkPawnOwnership(char player, int start_row, int start_column);
bool checkMovePlayer(int start_row, int start_column, int end_row, int end_column);
void makeMove(int start_row, int start_column, int end_row, int end_column);
void parseMove(const char* move, int* startRow, int* startColumn, int* endRow, int* endColumn);
void playerMove(int start_row, int start_column, int end_row, int end_column);
bool hasValidMoves(char player);
bool hasPawns(char player);
char checkWinner();
void printWinner(char winner);
int evaluateBoard();
int minimax(int depth, bool isMaximizing);
void computerMove();

#endif //HEXAPAWN_H
