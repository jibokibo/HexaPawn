# Hexapawn Project
This project implements the game Hexapawn, where the player competes against a computer on a 3x3 board.
The game is played entirely through the command line.
The computer uses the Minimax algorithm with optional Alpha-Beta Pruning to calculate optimal moves.

There are two versions of the game (one with Alpha-Beta Pruning and one without),
which are located in separate directories and must be compiled and run separately.

Game Rules:
Hexapawn is a simple strategy game with the following basic rules:
	•	Two players each have three pawns.
	•	The goal is to either reach the opponent’s back row,
capture all opposing pawns, or place the opponent in a position where no valid moves are possible.

Requirements:
	•	C compiler (e.g., GCC)
	•	Operating system with terminal access (macOS, Linux, Windows)

Installation and Execution (in a Unix terminal):
	1.	Compile the program:
gcc main.c hexapawn.c -o hexapawn
	2.	Run the program:
./hexapawn
