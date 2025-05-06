//ohne Alpha-Beta Pruning
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "hexapawn.h"



clock_t start, end;
double cpuTimeUsed;
int nodeCount;
char board[3][3]; // Board representation
const char HUMAN = 'W'; // Human player (White)
const char COMPUTER = 'B'; // Computer player (Black)






// Converts column (A, B, C) to index (0, 1, 2)
int columnToIndex(char column) {
    column = toupper(column);
    if (column == 'A') return 0;
    if (column == 'B') return 1;
    if (column == 'C') return 2;
    return -1; // Invalid column
}

// Converts row (1, 2, 3) to index (0, 1, 2)
int rowToIndex(int row) {
    if (row >= 1 && row <= 3) return row - 1;
    return -1; // Invalid row
}

// Initialize the board with players in their initial positions
void resetBoard() {
    for (int column = 0; column < 3; column++) {
        board[0][column] = COMPUTER; // First row for computer
        board[1][column] = ' '; // Middle row is empty
        board[2][column] = HUMAN; // Last row for player
    }
}

// Display the current state of the board
void printBoard() {
    printf("  A   B   C \n");
    printf("1 %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf(" ---|---|---\n");
    printf("2 %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf(" ---|---|---\n");
    printf("3 %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}

// Check if the destination coordinates are within the board limits
bool checkDestination(int end_row, int end_column) {
    return end_row >= 0 && end_row < 3 && end_column >= 0 && end_column < 3;
}

// Check if the given cell contains the player's pawn
int checkPawnOwnership(char player, int start_row, int start_column) {
    if(board[start_row][start_column] == player) return 1;
    else if (0 <= start_row && start_row <= 2 && 0 <= start_column && start_column <= 2) return 0;
    else return -1;

}

// Check if the player's move is valid (either forward or diagonal capture)
bool checkMovePlayer(int start_row, int start_column, int end_row, int end_column) {
    if (!checkPawnOwnership(HUMAN, start_row, start_column)) {
        fprintf(stderr, "Not your pawn or blank space\n");
        return false;
    }
    if (!checkDestination(end_row, end_column)) {
        fprintf(stderr, "Destination out of bounds\n");
        return false;
    }
    if (end_column == start_column && end_row == start_row - 1 && board[end_row][end_column] == ' ') {
        return true; // Forward move
    }
    if (abs(end_column - start_column) == 1 && end_row == start_row - 1) {
        if (board[end_row][end_column] == COMPUTER) {
            return true; // Diagonal capture
        }
    }
    return false;
}

// Perform the move by updating the board
void makeMove(int start_row, int start_column, int end_row, int end_column) {
    board[end_row][end_column] = board[start_row][start_column];
    board[start_row][start_column] = ' ';
}

// Parse the move in the format "A3-A2"
void parseMove(const char* move, int* startRow, int* startColumn, int* endRow, int* endColumn) {
    char startCol, endCol;
    int startR, endR;
    sscanf(move, "%c%d-%c%d", &startCol, &startR, &endCol, &endR);
    *startColumn = columnToIndex(startCol);
    *startRow = rowToIndex(startR);
    *endColumn = columnToIndex(endCol);
    *endRow = rowToIndex(endR);
}

// Handle player's move input and ensure it's valid
void playerMove(int start_row, int start_column, int end_row, int end_column) {
    do {
        printf("Enter your move (e.g. A3-A2): ");
        char move[6];
        scanf("%s", move);
        parseMove(move, &start_row, &start_column, &end_row, &end_column);
        if (!checkMovePlayer(start_row, start_column, end_row, end_column)) {
            fprintf(stderr, "Invalid move! Try again...\n");
        }
    } while (!checkMovePlayer(start_row, start_column, end_row, end_column));
    makeMove(start_row, start_column, end_row, end_column);
}

// Check if a player has any valid moves left
bool hasValidMoves(char player) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == player) {
                if (player == HUMAN) {
                    // Check forward move
                    if (i - 1 >= 0 && board[i - 1][j] == ' ') return true;
                    // Check diagonal captures
                    if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == COMPUTER) return true;
                    if (i - 1 >= 0 && j + 1 < 3 && board[i - 1][j + 1] == COMPUTER) return true;
                } else if (player == COMPUTER) {
                    // Check forward move
                    if (i + 1 < 3 && board[i + 1][j] == ' ') return true;
                    // Check diagonal captures
                    if (i + 1 < 3 && j - 1 >= 0 && board[i + 1][j - 1] == HUMAN) return true;
                    if (i + 1 < 3 && j + 1 < 3 && board[i + 1][j + 1] == HUMAN) return true;
                }
            }
        }
    }
    return false; // No valid moves left
}

// Check if a player has any pawns left
bool hasPawns(char player) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == player) return true;
        }
    }
    return false; // No pawns left
}

// Check if the game has a winner
char checkWinner() {
    // Check if a player has reached the opponent's baseline
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == HUMAN) return HUMAN; // Human reaches top row
        if (board[2][i] == COMPUTER) return COMPUTER; // Computer reaches bottom row
    }

    // Check if either player has no pawns left
    if (!hasPawns(HUMAN)) return COMPUTER; // Human has no pawns, computer wins
    if (!hasPawns(COMPUTER)) return HUMAN; // Computer has no pawns, human wins

    // Check if either player has no valid moves left
    bool humanHasMoves = hasValidMoves(HUMAN);
    bool computerHasMoves = hasValidMoves(COMPUTER);
    if (!computerHasMoves) return HUMAN; // Computer has no moves, human wins
    else if (!humanHasMoves) return COMPUTER; // Human has no moves, computer wins


    return ' '; // No winner yet
}

// Print the winner of the game
void printWinner(char winner) {
    if (winner == HUMAN) {
        printf("YOU WIN!\n");
    } else if (winner == COMPUTER) {
        printf("YOU LOSE!\n");
    }
}

// Evaluate the board state for the Minimax algorithm
int evaluateBoard() {
    char winner = checkWinner();
    if (winner == COMPUTER) return 10; // Computer wins
    if (winner == HUMAN) return -10; // Human wins

    // Evaluate based on pawn positions
    int humanPawns = 0, computerPawns = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == HUMAN) humanPawns++;
            if (board[i][j] == COMPUTER) computerPawns++;
        }
    }
    return computerPawns - humanPawns; // Favor computer
}

// Minimax algorithm without alpha-beta pruning
int minimax(int depth, bool isMaximizing) {
  nodeCount++;
  char winner = checkWinner();
    if (winner != ' ') {
        return evaluateBoard();
    }

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == COMPUTER) {
                    // Check forward move
                    if (i + 1 < 3 && board[i + 1][j] == ' ') {
                        board[i + 1][j] = COMPUTER;
                        board[i][j] = ' ';
                        int eval = minimax(depth + 1, false);
                        board[i][j] = COMPUTER;
                        board[i + 1][j] = ' ';
                        maxEval = (eval > maxEval) ? eval : maxEval;
                    }
                    // Check diagonal capture to the right
                    if (i + 1 < 3 && j + 1 < 3 && board[i + 1][j + 1] == HUMAN) {
                        board[i + 1][j + 1] = COMPUTER;
                        board[i][j] = ' ';
                        int eval = minimax(depth + 1, false);
                        board[i][j] = COMPUTER;
                        board[i + 1][j + 1] = HUMAN;
                        maxEval = (eval > maxEval) ? eval : maxEval;
                    }
                    // Check diagonal capture to the left
                    if (i + 1 < 3 && j - 1 >= 0 && board[i + 1][j - 1] == HUMAN) {
                        board[i + 1][j - 1] = COMPUTER;
                        board[i][j] = ' ';
                        int eval = minimax(depth + 1, false);
                        board[i][j] = COMPUTER;
                        board[i + 1][j - 1] = HUMAN;
                        maxEval = (eval > maxEval) ? eval : maxEval;
                    }
                }
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == HUMAN) {
                    // Check forward move
                    if (i - 1 >= 0 && board[i - 1][j] == ' ') {
                        board[i - 1][j] = HUMAN;
                        board[i][j] = ' ';
                        int eval = minimax(depth + 1, true);
                        board[i][j] = HUMAN;
                        board[i - 1][j] = ' ';
                        minEval = (eval < minEval) ? eval : minEval;
                    }
                    // Check diagonal capture to the right
                    if (i - 1 >= 0 && j + 1 < 3 && board[i - 1][j + 1] == COMPUTER) {
                        board[i - 1][j + 1] = HUMAN;
                        board[i][j] = ' ';
                        int eval = minimax(depth + 1, true);
                        board[i][j] = HUMAN;
                        board[i - 1][j + 1] = COMPUTER;
                        minEval = (eval < minEval) ? eval : minEval;
                    }
                    // Check diagonal capture to the left
                    if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == COMPUTER) {
                        board[i - 1][j - 1] = HUMAN;
                        board[i][j] = ' ';
                        int eval = minimax(depth + 1, true);
                        board[i][j] = HUMAN;
                        board[i - 1][j - 1] = COMPUTER;
                        minEval = (eval < minEval) ? eval : minEval;
                    }
                }
            }
        }
        return minEval;
    }
}

// Computer makes a move using the Minimax algorithm
void computerMove() {
    int bestEval = INT_MIN;
    int bestMove[4] = {-1, -1, -1, -1}; // startRow, startCol, endRow, endCol
    start = clock();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == COMPUTER) {
                // Check forward move
                if (i + 1 < 3 && board[i + 1][j] == ' ') {
                    board[i + 1][j] = COMPUTER;
                    board[i][j] = ' ';
                    int eval = minimax(0, false);
                    board[i][j] = COMPUTER;
                    board[i + 1][j] = ' ';
                    if (eval > bestEval) {
                        bestEval = eval;
                        bestMove[0] = i;
                        bestMove[1] = j;
                        bestMove[2] = i + 1;
                        bestMove[3] = j;
                    }
                }
                // Check diagonal capture to the right
                if (i + 1 < 3 && j + 1 < 3 && board[i + 1][j + 1] == HUMAN) {
                    board[i + 1][j + 1] = COMPUTER;
                    board[i][j] = ' ';
                    int eval = minimax(0, false);
                    board[i][j] = COMPUTER;
                    board[i + 1][j + 1] = HUMAN;
                    if (eval > bestEval) {
                        bestEval = eval;
                        bestMove[0] = i;
                        bestMove[1] = j;
                        bestMove[2] = i + 1;
                        bestMove[3] = j + 1;
                    }
                }
                // Check diagonal capture to the left
                if (i + 1 < 3 && j - 1 >= 0 && board[i + 1][j - 1] == HUMAN) {
                    board[i + 1][j - 1] = COMPUTER;
                    board[i][j] = ' ';
                    int eval = minimax(0, false);
                    board[i][j] = COMPUTER;
                    board[i + 1][j - 1] = HUMAN;
                    if (eval > bestEval) {
                        bestEval = eval;
                        bestMove[0] = i;
                        bestMove[1] = j;
                        bestMove[2] = i + 1;
                        bestMove[3] = j - 1;
                    }
                }
            }
        }
    }
    end = clock();
    cpuTimeUsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    if (bestMove[0] != -1) {
        makeMove(bestMove[0], bestMove[1], bestMove[2], bestMove[3]);
    }
}
