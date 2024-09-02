#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

#define W 60
#define H 30
#define U 72
#define D 80
#define L 75
#define R 77
#define ARROW 224
#define ESC 27

#define MAXBODY 300

struct COORDINATE
{
    int x;
    int y;
};

// Represent different types of food
struct FOOD
{
    COORDINATE position;
    char symbol;
    int value;
};

// Make one move towards the head
void moveBack(COORDINATE body[], int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        body[i].x = body[i - 1].x;
        body[i].y = body[i - 1].y;
    }
}

// Check if the snake has collided with itself
bool checkSelfCollision(COORDINATE body[], int size)
{
    for (int i = 1; i < size; i++)
    {
        if (body[0].x == body[i].x && body[0].y == body[i].y)
        {
            return true;
        }
    }
    return false;
}

// Generate different type of food on the board
void generateFood(FOOD &food)
{
    food.position.x = rand() % (W - 2) + 1; // Position food within walls
    food.position.y = rand() % (H - 2) + 1;

    // Randomize food type
    int type = rand() % 3;
    switch (type)
    {
    case 0:
        food.symbol = '@';
        food.value = 10; // Normal food
        break;
    case 1:
        food.symbol = '$';
        food.value = 20; // Bonus food
        break;
    case 2:
        food.symbol = '*';
        food.value = 30; // Super bonus food
        break;
    }
}

// Run the game for a single player
int runGame(int playerNum)
{
    system("cls"); // Clear console screen, start from empty

    COORDINATE body[MAXBODY];
    int bodylen = 3;
    body[0].x = W / 2; // Start with a body
    body[0].y = H / 2;
    char lastc = R; // First direction is Right
    int score = 0;  // Initialize score

    FOOD food;
    generateFood(food); // Generate initial food

    // Off-screen buffer to reduce flickering
    char screen[H][W];

    for (;;)
    {
        // Check key stroke
        if (_kbhit())
        {
            // Arrow key is a 2-key combination, 224-72 for "up"
            unsigned char c = _getch(); // Get first key
            if (c == ARROW)
            {
                c = _getch(); // Get second key
                switch (c)
                {
                case U:
                    lastc = U;
                    break; // Up
                case L:
                    lastc = L;
                    break; // Left
                case R:
                    lastc = R;
                    break; // Right
                case D:
                    lastc = D;
                    break; // Down
                }
            }
            else if (c == ESC) // ESC key
            {
                return score; // Return score if ESC is pressed
            }
        }

        // Move the snake in the direction of lastc
        switch (lastc)
        {
        case U:
            moveBack(body, bodylen);
            body[0].y -= 1;
            break; // Up
        case L:
            moveBack(body, bodylen);
            body[0].x -= 1;
            break; // Left
        case R:
            moveBack(body, bodylen);
            body[0].x += 1;
            break; // Right
        case D:
            moveBack(body, bodylen);
            body[0].y += 1;
            break; // Down
        }

        // Check for wall collision
        if (body[0].x <= 0 || body[0].x >= W - 1 || body[0].y <= 0 || body[0].y >= H - 1)
        {
            cout << "Player " << playerNum << " Game Over! You hit the wall." << endl;
            break;
        }

        // Check for self-collision
        if (checkSelfCollision(body, bodylen))
        {
            cout << "Player " << playerNum << " Game Over! You collided with yourself." << endl;
            break;
        }

        // Check if snake eats food
        if (body[0].x == food.position.x && body[0].y == food.position.y)
        {
            score += food.value;
            if (bodylen < MAXBODY)
                bodylen++; // Increase length only when food is eaten
            generateFood(food); // Generate new food
        }

        // Clear the screen buffer
        for (int r = 0; r < H; r++)
            for (int c = 0; c < W; c++)
                screen[r][c] = ' ';

        // Draw the walls
        for (int r = 0; r < H; r++)
        {
            screen[r][0] = screen[r][W - 1] = '#';
        }
        for (int c = 0; c < W; c++)
        {
            screen[0][c] = screen[H - 1][c] = '#';
        }

        // Draw the snake
        for (int i = 0; i < bodylen; i++)
        {
            screen[body[i].y][body[i].x] = 'X'; // Display 'X' for snake body
        }

        // Draw the food
        screen[food.position.y][food.position.x] = food.symbol;

        // Output the screen buffer to the console
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        for (int r = 0; r < H; r++)
        {
            for (int c = 0; c < W; c++)
            {
                cout << screen[r][c];
            }
            cout << endl;
        }

        // Speed control
        Sleep(100);
    }

    cout << "Player " << playerNum << " Score: " << score << endl;
	cout << "Press any key to continue" << endl;
    _getch(); // Pause before exit
    return score;
}

int main()
{
    srand(time(0)); // Initialize random seed

    int score1, score2;

    cout << "Player 1, get ready!" << endl;
    Sleep(2000); // 2-second delay to prepare
    score1 = runGame(1); // Run game for Player 1

    cout << "Player 2, get ready!" << endl;
    Sleep(2000); // 2-second delay to prepare
    score2 = runGame(2); // Run game for Player 2

    // Display final scores
    system("cls");
    cout << "Final Scores:" << endl;
    cout << "Player 1: " << score1 << endl;
    cout << "Player 2: " << score2 << endl;

    // Determine the winner
    if (score1 > score2)
        cout << "Player 1 wins!" << endl;
    else if (score2 > score1)
        cout << "Player 2 wins!" << endl;
    else
        cout << "It's a tie!" << endl;

    _getch(); // Pause before exit
    return 0;
}
