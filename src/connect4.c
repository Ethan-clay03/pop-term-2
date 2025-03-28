#include "raylib.h"

//Define how many columns and rows there should be
#define ROWS 6
#define COLUMNS 7
#define CELL_SIZE 100

enum Cell {
    EMPTY,
    PLAYER1,
    PLAYER2
};

enum Cell board[ROWS][COLUMNS] = { { EMPTY } };
enum Cell current_player = PLAYER1;
bool game_won = false;
char win_message[64] = "";
extern char player1Name[64];
extern char player2Name[64];

//Setup falling disc structure and set default below
typedef struct {
    bool active;
    int col;
    float y;
    Color color;
} FallingDisc;

FallingDisc fallingDisc = { false, 0, 0.0f, LIGHTGRAY };

void draw_background_grid() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            //Center X, Center Y, radius, colour
            DrawCircle(col * CELL_SIZE + CELL_SIZE / 2, row * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 2 - 5, LIGHTGRAY);
        }
    }
}

void draw_board() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            Color cell_color;
            if (board[row][col] == PLAYER1) {
                cell_color = RED;
            } else if (board[row][col] == PLAYER2) {
                cell_color = YELLOW;
            } else {
                continue;  // Skip empty cells; background will handle this
            }
            DrawCircle(col * CELL_SIZE + CELL_SIZE / 2, row * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 2 - 5, cell_color);
        }
    }
}


bool check_win(enum Cell player) {
    // Horizontal
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS - 3; col++) {
            if (board[row][col] == player && board[row][col + 1] == player && board[row][col + 2] == player && board[row][col + 3] == player) {
                return true;
            }
        }
    }
    // Vertical
    for (int row = 0; row < ROWS - 3; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (board[row][col] == player && board[row + 1][col] == player && board[row + 2][col] == player && board[row + 3][col] == player) {
                return true;
            }
        }
    }
    // Top Left to Bottom Right
    for (int row = 0; row < ROWS - 3; row++) {
        for (int col = 0; col < COLUMNS - 3; col++) {
            if (board[row][col] == player && board[row + 1][col + 1] == player && board[row + 2][col + 2] == player && board[row + 3][col + 3] == player) {
                return true;
            }
        }
    }
    // Top Right to Bottom Left
    for (int row = 3; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS - 3; col++) {
            if (board[row][col] == player && board[row - 1][col + 1] == player && board[row - 2][col + 2] == player && board[row - 3][col + 3] == player) {
                return true;
            }
        }
    }
    return false;
}


void AnimateFallingDisc(int target_row, int target_col, float *falling_y_position) {
    // Move the disc down by 10 units in each update
    *falling_y_position += 10.0f;

    // Check if disc is at target
    if (*falling_y_position >= target_row * CELL_SIZE + CELL_SIZE / 2) {
        board[target_row][target_col] = current_player;

        if (check_win(current_player)) {
            game_won = true;
            sprintf(win_message, "%s Wins!", current_player == PLAYER1 ? player1Name : player2Name);
        }

        current_player = (current_player == PLAYER1) ? PLAYER2 : PLAYER1;
        // Reset the falling animation for the next turn
        *falling_y_position = -1.0f;
    } else {
        // Draw the disc at the current falling position
        DrawCircle(target_col * CELL_SIZE + CELL_SIZE / 2, *falling_y_position, CELL_SIZE / 2 - 5, 
                   (current_player == PLAYER1) ? RED : YELLOW);
    }
}


void gameStart() {
    static int animated_row = -1;
    static int animated_col = -1; 
    static float animated_y = 0.0f;

    if (animated_row >= 0) {
        AnimateFallingDisc(animated_row, animated_col, &animated_y);

        // If disc landed reset state
        if (animated_y == -1.0f) {
            animated_row = -1;
        }
    } else if (!game_won && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        //Column on mouse click
        int column = GetMouseX() / CELL_SIZE;

        // Find the first empty row in the selected column (starting from the bottom)
        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][column] == EMPTY) {
                // Set the row and column where the disc will fall
                animated_row = row;
                animated_col = column;
                animated_y = 0.0f;  // Reset the Y position to begin animation
                break;
            }
        }
    }

    draw_background_grid();
    if (animated_row >= 0) {
        DrawCircle(animated_col * CELL_SIZE + CELL_SIZE / 2, animated_y, CELL_SIZE / 2 - 5, 
                   (current_player == PLAYER1) ? RED : YELLOW);
    }
    draw_board();

    //Show game win message
    if (game_won) {
        DrawText(win_message, 10, ROWS * CELL_SIZE + 10, 20, BLACK);
    }
}
