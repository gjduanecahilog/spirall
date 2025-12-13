#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// --- Game Constants ---
#define BOARD_SIZE 3
#define EMPTY_CELL ' '

// --- Global Game State ---
char board[BOARD_SIZE][BOARD_SIZE];
char current_player = 'X';
int moves_made = 0;

// --- Function Prototypes ---
void initialize_board();
void draw_board();
int check_win();
int check_tie();
void get_move();
void switch_player();

// --- Main Function ---
int main() {
    int game_over = 0;

    printf("*********************************\n");
    printf("*** Welcome to Tic-Tac-Toe! ***\n");
    printf("*********************************\n\n");

    initialize_board();

    // --- Main Game Loop ---
    while (!game_over) {
        draw_board();
        printf("Player %c's turn.\n", current_player);
        get_move();

        // Check Win/Tie conditions after a move
        if (check_win()) {
            draw_board();
            printf("\n*** CONGRATULATIONS! Player %c WINS! ***\n", current_player);
            game_over = 1;
        } else if (check_tie()) {
            draw_board();
            printf("\n--- IT'S A TIE! GAME OVER. ---\n");
            game_over = 1;
        } else {
            switch_player();
        }
    }

    return 0;
}

// --- Function Definitions ---

/**
 * Initializes the board grid with empty spaces.
 */
void initialize_board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY_CELL;
        }
    }
    current_player = 'X';
    moves_made = 0;
}

/**
 * Draws the current state of the board to the console.
 */
void draw_board() {
    printf("\n  0   1   2\n");
    printf(" +---+---+---+\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d|", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Display the cell content, defaulting to ' '
            printf(" %c |", board[i][j]);
        }
        printf("\n +---+---+---+\n");
    }
}

/**
 * Gets a valid move from the current player and updates the board.
 */
void get_move() {
    int row, col;
    int valid_move = 0;

    while (!valid_move) {
        printf("Enter row and column (e.g., 0 2): ");
        // Ensure scanf reads exactly two integers
        if (scanf("%d %d", &row, &col) != 2) {
            printf("Invalid input format. Please enter two numbers.\n");
            // Clear the input buffer on failure
            while (getchar() != '\n');
            continue;
        }

        // 1. Check if coordinates are within bounds (0 to 2)
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            printf("Invalid coordinates. Row and column must be between 0 and 2.\n");
            continue;
        }

        // 2. Check if the cell is already occupied
        if (board[row][col] != EMPTY_CELL) {
            printf("That cell is already taken. Try again.\n");
            continue;
        }

        // If checks pass, the move is valid
        board[row][col] = current_player;
        moves_made++;
        valid_move = 1;
    }
}

/**
 * Checks all rows, columns, and diagonals for a win condition.
 * Returns 1 if the current player has won, 0 otherwise.
 */
int check_win() {
    // --- 1. Check Rows and Columns ---
    for (int i = 0; i < BOARD_SIZE; i++) {
        // Check Row i: board[i][0], board[i][1], board[i][2]
        if (board[i][0] != EMPTY_CELL && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return 1;
        }
        // Check Column i: board[0][i], board[1][i], board[2][i]
        if (board[0][i] != EMPTY_CELL && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return 1;
        }
    }

    // --- 2. Check Diagonals ---

    // Top-left to bottom-right (board[0][0], board[1][1], board[2][2])
    if (board[0][0] != EMPTY_CELL && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return 1;
    }

    // Top-right to bottom-left (board[0][2], board[1][1], board[2][0])
    if (board[0][2] != EMPTY_CELL && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return 1;
    }

    return 0; // No win found
}

/**
 * Checks if all possible moves (9) have been made.
 * Returns 1 if the game is a tie, 0 otherwise.
 */
int check_tie() {
    return (moves_made == BOARD_SIZE * BOARD_SIZE);
}

/**
 * Switches the current player from 'X' to 'O' or vice-versa.
 */
void switch_player() {
    current_player = (current_player == 'X') ? 'O' : 'X';
}
