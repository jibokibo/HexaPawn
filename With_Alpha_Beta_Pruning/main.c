// mit Alpha-Beta Pruning
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "hexapawn.h"

int main() {
    char winner;
    char response;
    do {
        winner = ' ';
        response = ' ';
        resetBoard();
        int i = 1;
        while (winner == ' ') {
            printBoard();

            // Player's move
            int startRow, startColumn, endRow, endColumn;
            playerMove(startRow, startColumn, endRow, endColumn);
            winner = checkWinner();
            if (winner != ' ') break;

            // Computer's move
            computerMove();
            winner = checkWinner();
        }

        printBoard();
        printWinner(winner);

        printf("\nWould you like to play again? (Y/N): ");
        scanf(" %c", &response);
        response = toupper(response);
    } while (response == 'Y');
    fclose(fp);
    return 0;
}