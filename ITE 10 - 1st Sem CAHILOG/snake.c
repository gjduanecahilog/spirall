#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // For memset

#ifdef _WIN32
#include <windows.h> // For Sleep
#define CLEAR_SCREEN "cls"
#else
#include <unistd.h> // For usleep
#define CLEAR_SCREEN "clear"
#endif

// --- Game Constants ---
#define BOARD_WIDTH 20
#define BOARD_HEIGHT 15
#define INITIAL_SNAKE_LENGTH 3
#define MAX_SNAKE_LENGTH (BOARD_WIDTH * BOARD_HEIGHT) // Max possible length
#define GAME_SPEED_MS 200 // Milliseconds between updates (higher = slower)

// --- Structs ---
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    int length;
    int dx, dy; // Direction vector (change in x, change in y)
} Snake;

// --- Global Game State ---
char game_board[BOARD_HEIGHT][BOARD_WIDTH];
Snake snake;
Point food;
int score = 0;
int game_over = 0;

// --- Function Prototypes ---
void initialize_game();
void draw_board();
void generate_food();
void handle_input();
void update_game();
void clear_screen();
void game_loop();

// --- Main Function ---
int main() {
    srand(time(NULL)); // Seed random number generator
    initialize_game();
    game_loop();

    printf("\n--- GAME OVER ---\n");
    printf("Final Score: %d\n", score);
    printf("-------------------\n");

    return 0;
}

// --- Function Definitions ---

void clear_screen() {
    system(CLEAR_SCREEN);
}

void initialize_game() {
    // Initialize board with spaces
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            game_board[i][j] = ' ';
        }
    }

    // Initialize snake
    snake.length = INITIAL_SNAKE_LENGTH;
    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = BOARD_WIDTH / 2 - i; // Start from middle, moving left
        snake.body[i].y = BOARD_HEIGHT / 2;
    }
    snake.dx = 1; // Initial direction: Right
    snake.dy = 0;

    generate_food();
}

void draw_board() {
    clear_screen();

    // Draw top border
    for (int j = 0; j < BOARD_WIDTH + 2; j++) printf("#");
    printf("\n");

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        printf("#"); // Left border
        for (int j = 0; j < BOARD_WIDTH; j++) {
            char cell_char = ' ';

            // Check if it's food
            if (food.x == j && food.y == i) {
                cell_char = 'F'; // Food representation
            }
            // Check if it's snake body
            for (int k = 0; k < snake.length; k++) {
                if (snake.body[k].x == j && snake.body[k].y == i) {
                    cell_char = (k == 0) ? 'O' : 'o'; // 'O' for head, 'o' for body
                    break;
                }
            }
            printf("%c", cell_char);
        }
        printf("#\n"); // Right border
    }

    // Draw bottom border
    for (int j = 0; j < BOARD_WIDTH + 2; j++) printf("#");
    printf("\n");

    printf("Score: %d\n", score);
    printf("Controls: W (Up), A (Left), S (Down), D (Right). Press Enter after each.\n");
}

void generate_food() {
    int attempts = 0;
    while (attempts < MAX_SNAKE_LENGTH * 2) { // Prevent infinite loop on full board
        food.x = rand() % BOARD_WIDTH;
        food.y = rand() % BOARD_HEIGHT;

        // Check if food spawns on snake
        int on_snake = 0;
        for (int i = 0; i < snake.length; i++) {
            if (snake.body[i].x == food.x && snake.body[i].y == food.y) {
                on_snake = 1;
                break;
            }
        }
        if (!on_snake) {
            return; // Food successfully placed
        }
        attempts++;
    }
    // If we can't find a spot, it means the board is full (game over due to win)
    game_over = 1;
}

void handle_input() {
    char input_char;
    if (scanf(" %c", &input_char) != 1) {
        // Handle input error or no input
        return;
    }
    input_char = tolower(input_char); // Convert to lowercase

    // Update direction based on input, prevent 180-degree turns
    switch (input_char) {
        case 'w': // Up
            if (snake.dy == 0) { snake.dx = 0; snake.dy = -1; }
            break;
        case 's': // Down
            if (snake.dy == 0) { snake.dx = 0; snake.dy = 1; }
            break;
        case 'a': // Left
            if (snake.dx == 0) { snake.dx = -1; snake.dy = 0; }
            break;
        case 'd': // Right
            if (snake.dx == 0) { snake.dx = 1; snake.dy = 0; }
            break;
        case 'q': // Quit
            game_over = 1;
            break;
    }
    // Clear any remaining input characters in the buffer
    while (getchar() != '\n' && !feof(stdin));
}

void update_game() {
    // 1. Calculate new head position
    Point new_head = snake.body[0];
    new_head.x += snake.dx;
    new_head.y += snake.dy;

    // 2. Check for collisions
    // Wall collision
    if (new_head.x < 0 || new_head.x >= BOARD_WIDTH ||
        new_head.y < 0 || new_head.y >= BOARD_HEIGHT) {
        game_over = 1;
        return;
    }

    // Self-collision (start from body[1] to avoid head-to-head check)
    for (int i = 1; i < snake.length; i++) {
        if (new_head.x == snake.body[i].x && new_head.y == snake.body[i].y) {
            game_over = 1;
            return;
        }
    }

    // 3. Move snake body
    // Shift all body segments one position down (from tail to head)
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0] = new_head; // Place new head

    // 4. Check for food eaten
    if (new_head.x == food.x && new_head.y == food.y) {
        score += 10;
        snake.length++; // Grow snake
        if (snake.length > MAX_SNAKE_LENGTH) { // Win condition if board full
            game_over = 1;
            printf("You filled the entire board! Amazing!\n");
            return;
        }
        generate_food(); // Spawn new food
    }
}

void game_loop() {
    while (!game_over) {
        draw_board();
        handle_input(); // This version waits for Enter
        update_game();

        // Optional: Introduce a small delay to control game speed
        #ifdef _WIN32
        // Sleep(GAME_SPEED_MS);
        #else
        // usleep(GAME_SPEED_MS * 1000); // usleep takes microseconds
        #endif
    }
}
