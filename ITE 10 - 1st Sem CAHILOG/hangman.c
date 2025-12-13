#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Global Constants ---
#define MAX_WORDS 10
#define MAX_WORD_LENGTH 15
#define MAX_TRIES 6

// --- Function Prototypes ---
void initialize_game(char *word, char *known_word, int *word_length);
void draw_hangman(int tries_left);
int process_guess(char guess, const char *word, char *known_word, char *guessed_letters);
void update_guessed_letters(char guess, char *guessed_letters);

// --- Main Function ---
int main() {
    // Seed the random number generator
    srand(time(NULL));

    char secret_word[MAX_WORD_LENGTH];
    char known_word[MAX_WORD_LENGTH];
    int word_length;
    int tries_left = MAX_TRIES;
    char guess;
    char guessed_letters[27] = {0}; // Array to store all guessed letters (a-z)

    printf("*********************************\n");
    printf("*** Welcome to Console Hangman! ***\n");
    printf("*********************************\n\n");

    // 1. Setup the game
    initialize_game(secret_word, known_word, &word_length);
    printf("The word has %d letters.\n\n", word_length);

    //

    // --- Main Game Loop ---
    while (tries_left > 0) {
        printf("\n--- Round Status ---\n");
        draw_hangman(tries_left);
        printf("Word: %s\n", known_word);
        printf("Guessed Letters: %s\n", guessed_letters);
        printf("Tries Left: %d\n", tries_left);
        printf("--------------------\n");

        // 2. Get user input (guess)
        printf("Enter your guess (a single lowercase letter): ");
        if (scanf(" %c", &guess) != 1) {
            // Handle bad input (e.g., EOF)
            while(getchar() != '\n'); // Clear buffer
            continue;
        }

        // Convert guess to lowercase
        guess = tolower(guess);

        // Basic input validation: check if it's a letter
        if (guess < 'a' || guess > 'z') {
            printf("Invalid input. Please enter a letter (a-z).\n");
            continue;
        }

        // Check if the letter has already been guessed
        if (strchr(guessed_letters, guess) != NULL) {
            printf("You already guessed the letter '%c'. Try another one!\n", guess);
            continue;
        }

        // 3. Process the guess
        int match_found = process_guess(guess, secret_word, known_word, guessed_letters);

        if (match_found) {
            printf("Good guess! '%c' is in the word.\n", guess);
        } else {
            tries_left--;
            printf("Incorrect guess. '%c' is NOT in the word. You lose a try!\n", guess);
        }

        // 4. Check Win/Loss conditions

        // Check for Win: If known_word equals secret_word
        if (strcmp(secret_word, known_word) == 0) {
            printf("\n*********************************\n");
            printf("*** CONGRATULATIONS! YOU WON! ***\n");
            printf("The word was: %s\n", secret_word);
            printf("*********************************\n");
            return 0; // End game
        }
    }

    // Game Over (Loss)
    printf("\n--- GAME OVER ---\n");
    draw_hangman(0); // Draw the fully hanged man
    printf("You ran out of tries! The secret word was: %s\n", secret_word);
    printf("*****************\n");

    return 0;
}

// --- Function Definitions ---

/**
 * Selects a random word and initializes the known_word (with underscores).
 */
void initialize_game(char *word, char *known_word, int *word_length) {
    const char *word_list[MAX_WORDS] = {
        "computer", "programming", "developer", "keyboard", "language",
        "hangman", "terminal", "console", "algorithm", "variable"
    };

    // Pick a random word from the list
    int random_index = rand() % MAX_WORDS;
    strcpy(word, word_list[random_index]);

    *word_length = strlen(word);

    // Initialize known_word with underscores (_)
    for (int i = 0; i < *word_length; i++) {
        known_word[i] = '_';
    }
    known_word[*word_length] = '\0'; // Null-terminate the string
}

/**
 * Draws the hangman figure based on the number of tries left.
 */
void draw_hangman(int tries_left) {
    printf("  +---+\n");
    printf("  |   |\n");
    printf("  %c   |\n", (tries_left <= 5) ? 'O' : ' ');  // Head (Try 6)
    printf(" %c%c%c  |\n",
        (tries_left <= 3) ? '/' : ' ',                  // Left Arm (Try 4)
        (tries_left <= 4) ? '|' : ' ',                  // Body (Try 5)
        (tries_left <= 2) ? '\\' : ' '                   // Right Arm (Try 3)
    );
    printf("  %c   |\n", (tries_left <= 4) ? '|' : ' ');  // Lower Body (part of Body)
    printf(" %c %c  |\n",
        (tries_left <= 1) ? '/' : ' ',                  // Left Leg (Try 2)
        (tries_left <= 0) ? '\\' : ' '                   // Right Leg (Try 1)
    );
    printf("      |\n");
    printf("=========\n");
}

/**
 * Checks the guess against the secret word and updates the known_word.
 * Returns 1 if a match is found, 0 otherwise.
 */
int process_guess(char guess, const char *word, char *known_word, char *guessed_letters) {
    int match_found = 0;

    // 1. Check if the guess is in the secret word
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == guess) {
            known_word[i] = guess;
            match_found = 1;
        }
    }

    // 2. Update the list of all guessed letters
    update_guessed_letters(guess, guessed_letters);

    return match_found;
}

/**
 * Adds the new guess to the list of guessed letters and keeps them sorted.
 */
void update_guessed_letters(char guess, char *guessed_letters) {
    int len = strlen(guessed_letters);

    // Insert the new guess into the array
    guessed_letters[len] = guess;
    guessed_letters[len + 1] = '\0'; // Re-null-terminate

    // Simple Bubble Sort to keep the letters in alphabetical order for display
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len - i; j++) {
            if (guessed_letters[j] > guessed_letters[j + 1]) {
                char temp = guessed_letters[j];
                guessed_letters[j] = guessed_letters[j + 1];
                guessed_letters[j + 1] = temp;
            }
        }
    }
}
