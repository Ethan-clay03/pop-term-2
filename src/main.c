#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include "connect4.c"

// Variables for main menu
bool showMainMenu = true;
bool showNameScreen = false;
char player1Name[64] = "";
char player2Name[64] = "";
bool enterPlayer1Name = true;


/**
 * Check if player names are empty. If so set default names
 */
void validateGameNames() {
    if (strlen(player1Name) == 0) {
        strcpy(player1Name, "Player 1");
    }
    if (strlen(player2Name) == 0) {
        strcpy(player2Name, "Player 2");
    }
}

/**
 * Checks if button passed through is being hovered over. Returns true if cursor is over button
 */
bool IsMouseOverButton(Rectangle button) {
    Vector2 mousePoint = GetMousePosition();
    return CheckCollisionPointRec(mousePoint, button);
}

// Allow users to enter a username and display that to the screen.
void HandlePlayerNameInput() {
    int key = GetKeyPressed();
    char* currentPlayerName = enterPlayer1Name ? player1Name : player2Name;
    if (key >= 32 && key <= 126) {
        int len = strlen(currentPlayerName);
        if (len < 63) {
            currentPlayerName[len] = (char)key;
            currentPlayerName[len + 1] = '\0';
        }
    }
    if (IsKeyPressed(KEY_ENTER)) {
        //Set flag to false to show player 2
        if (enterPlayer1Name) {
            enterPlayer1Name = false;
        //Second enter is player 2, show game screen
        } else {
            showNameScreen = false;
        }
    }
    //Remove last character from the string by adding null terminator
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(currentPlayerName);
        if (len > 0) {
            currentPlayerName[len - 1] = '\0';
        }
    }
}

void ShowMainMenu() {
    Rectangle startButton = { 100, 150, 325, 40 };
    Rectangle nameButton = { 100, 200, 325, 40 };


    DrawText("Connect 4 Game", 100, 100, 20, BLACK);

    if (IsMouseOverButton(startButton)) {
        //Draw new rectangle different colour to give hover affect
        DrawRectangleRec(startButton, LIGHTGRAY);
        //If 'start' button is clicked start game without selecting usernames
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showMainMenu = false;
        }
    } else {
        //If button is not hovered keep darker gray button on screen
        DrawRectangleRec(startButton, GRAY);
    }

    //Same as above but for name selection button
    if (IsMouseOverButton(nameButton)) {
        DrawRectangleRec(nameButton, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showMainMenu = false;
            showNameScreen = true;
        }
    } else {
        DrawRectangleRec(nameButton, GRAY);
    }
    DrawText("Start Game (without names)", 110, 160, 20, BLACK);
    DrawText("Start Game (name selection)", 110, 210, 20, BLACK);
}

//Show name screen, allow users to enter username
void ShowNameScreen() {
    DrawText("Enter Player Names", 100, 100, 20, BLACK);
    if (enterPlayer1Name) {
        DrawText("Enter Player 1 Name:", 100, 150, 20, BLACK);
        DrawText(player1Name, 350, 150, 20, BLACK);
    } else {
        DrawText("Enter Player 2 Name:", 100, 150, 20, BLACK);
        DrawText(player2Name, 350, 150, 20, BLACK);
    }
    HandlePlayerNameInput();
}

int main(void) {
    InitWindow(COLUMNS * CELL_SIZE, ROWS * CELL_SIZE + 100, "Connect 4");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (showMainMenu) {
            ShowMainMenu();
        } else if (showNameScreen) {
            ShowNameScreen();
        } else {
            validateGameNames();
            gameStart();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
