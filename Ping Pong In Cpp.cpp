/*
Author: Saeed Soukiah
Date: 19/12/2024
*/

#include <iostream>
#include <raylib.h>

// Enum to define game states
enum GameState { MENU, PLAYING, GAME_OVER };
GameState currentState = MENU; // Initial game state is MENU

// Global Variables
int playerScore = 0;
int cpuScore = 0;
const int screenWidth = 1200;
const int screenHeight = 800;

// Colors for the game elements
Color Green = { 38, 185, 154, 255 };
Color Dark_Green = { 20, 160, 133, 255 };
Color Light_Green = { 129, 204, 184, 255 };
Color Yellow = { 243, 213, 91, 255 };

// Sound Variables
Sound collisionSound;
Sound scoreSound;

// Ball Class
class Ball {
public:
    float x, y;
    float speedX, speedY;
    int radius;

    // Constructor Initializer List to initialize ball properties
    Ball(float posX, float posY, float spdX, float spdY, int rad)
        : x(posX), y(posY), speedX(spdX), speedY(spdY), radius(rad) {
    }
    
    // Method to draw the ball
    void draw() {
        DrawCircle(static_cast<int>(x), static_cast<int>(y), radius, Yellow);
    }

    // Method to update ball position and handle collisions
    void update() {
        x += speedX;
        y += speedY;

        // Check for collision with top and bottom walls
        if (y + radius >= screenHeight || y - radius <= 0) {
            speedY *= -1;
            PlaySound(collisionSound); // Play collision sound
        }

        // Check for scoring conditions
        if (x + radius >= screenWidth) { // CPU scores
            cpuScore++;
            PlaySound(scoreSound); // Play score sound
            reset();
        }
        if (x - radius <= 0) { // Player scores
            playerScore++;
            PlaySound(scoreSound); // Play score sound
            reset();
        }
    }

    // Method to reset ball position and speed
    void reset() {
        x = screenWidth / 2;
        y = screenHeight / 2;
        speedX = (GetRandomValue(0, 1) == 0) ? -7.0f : 7.0f;
        speedY = (GetRandomValue(0, 1) == 0) ? -7.0f : 7.0f;
    }
};

// Paddle Class
class Paddle {
protected:
    // Method to ensure paddle stays within screen boundaries
    void limitMovement() {
        if (y <= 0) y = 0;
        if (y + height >= screenHeight) y = screenHeight - height;
    }

public:
    float x, y;
    float width, height;
    int speed;

    // Constructor to initialize paddle properties
    Paddle(float posX, float posY, float w, float h, int spd)
        : x(posX), y(posY), width(w), height(h), speed(spd) {
    }

    // Method to draw the paddle
    void draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8f, 0, WHITE);
    }

    // Method to update paddle position based on user input
    void update() {
        if (IsKeyDown(KEY_UP)) y -= speed;
        if (IsKeyDown(KEY_DOWN)) y += speed;
        limitMovement();
    }
};

// CPU Paddle Class (inherits from Paddle)
class CpuPaddle : public Paddle {
public:
    // Constructor to initialize CPU paddle properties
    CpuPaddle(float posX, float posY, float w, float h, int spd)
        : Paddle(posX, posY, w, h, spd) {
    }

    // Method to update CPU paddle position based on ball position
    void update(int ballY) {
        if (y + height / 2 > ballY) y -= speed;
        if (y + height / 2 < ballY) y += speed;
        limitMovement();
    }
};

// Function Declarations
void DrawGame(Ball& ball, Paddle& player, CpuPaddle& cpu);
void UpdateGame(Ball& ball, Paddle& player, CpuPaddle& cpu);
bool CheckCollisionCircleRec(Vector2 circlePos, float radius, Rectangle rec);
void ResetScores();
void DrawMenu();
void DrawGameOver();

// Main Function
void main() {
    InitWindow(screenWidth, screenHeight, "Pong Game");
    InitAudioDevice(); // Initialize audio device
    SetTargetFPS(60);

    // Load sound effects
    collisionSound = LoadSound("collision.wav");
    scoreSound = LoadSound("score.wav");

    // Check if sound files are loaded correctly
    if (!collisionSound.stream.buffer) {
        std::cerr << "Error loading collision.wav" << std::endl;
    }
    if (!scoreSound.stream.buffer) {
        std::cerr << "Error loading score.wav" << std::endl;
    }

    // Initialize ball and paddles
    Ball ball(screenWidth / 2, screenHeight / 2, 7.0f, 7.0f, 20);
    Paddle player(screenWidth - 35, screenHeight / 2 - 60, 25, 120, 6);
    CpuPaddle cpu(10, screenHeight / 2 - 60, 25, 120, 6);

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Dark_Green);

        // Handle different game states
        switch (currentState) {
        case MENU:
            DrawMenu(); // Draw the menu screen
            if (IsKeyPressed(KEY_ENTER)) currentState = PLAYING;
            break;
        case PLAYING:
            UpdateGame(ball, player, cpu); // Update game elements
            DrawGame(ball, player, cpu); // Draw game elements
            if (playerScore >= 10 || cpuScore >= 10) currentState = GAME_OVER;
            break;
        case GAME_OVER:
            DrawGameOver(); // Draw game over screen
            if (IsKeyPressed(KEY_ENTER)) {
                ResetScores();
                currentState = MENU;
            }
            break;
        }

        EndDrawing();
    }

    // Unload sound effects and close audio device
    UnloadSound(collisionSound);
    UnloadSound(scoreSound);
    CloseAudioDevice();
    CloseWindow();
}

// Draw Game Elements
void DrawGame(Ball& ball, Paddle& player, CpuPaddle& cpu) {
    // Draw middle dividing line
    DrawRectangle(screenWidth / 2 - 1, 0, 2, screenHeight, GREEN);
    // Draw central circle
    DrawCircle(screenWidth / 2, screenHeight / 2, 150, Light_Green);
    // Draw dividing line
    DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
    // Draw the ball and paddles
    ball.draw();
    cpu.draw();
    player.draw();
    // Draw the scores
    DrawText(TextFormat("%i", cpuScore), screenWidth / 4, 20, 80, WHITE);
    DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4, 20, 80, WHITE);
}

// Update Game Elements
void UpdateGame(Ball& ball, Paddle& player, CpuPaddle& cpu) {
    // Update the ball and paddles
    ball.update();
    player.update();
    cpu.update(static_cast<int>(ball.y));

    // Check for collisions between the ball and the paddles
    if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, static_cast<float>(ball.radius), Rectangle{ player.x, player.y, player.width, player.height })) {
        ball.speedX *= -1;
        PlaySound(collisionSound); // Play collision sound
    }
    if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, static_cast<float>(ball.radius), Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
        ball.speedX *= -1;
        PlaySound(collisionSound); // Play collision sound
    }
}

// Check Collision Between Circle and Rectangle
bool CheckCollisionCircleRec(Vector2 circlePos, float radius, Rectangle rec) {
    // Find the closest point to the circle within the rectangle
    float closestX = fmaxf(rec.x, fminf(circlePos.x, rec.x + rec.width));
    float closestY = fmaxf(rec.y, fminf(circlePos.y, rec.y + rec.height));

    // Calculate the distance between the circle's center and this closest point
    float distanceX = circlePos.x - closestX;
    float distanceY = circlePos.y - closestY;

    // If the distance is less than the circle's radius, there is an intersection
    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}

// Reset Scores
void ResetScores() {
    playerScore = 0;
    cpuScore = 0;
}

// Draw Menu Screen
void DrawMenu() {
    DrawText("PONG GAME", screenWidth / 2 - MeasureText("PONG GAME", 60) / 2, screenHeight / 2 - 100, 60, WHITE);
    DrawText("Press ENTER to Start", screenWidth / 2 - MeasureText("Press ENTER to Start", 40) / 2, screenHeight / 2, 40, WHITE);
}

// Draw Game Over Screen
void DrawGameOver() {
    DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 60) / 2, screenHeight / 2 - 100, 60, WHITE);
    DrawText("Press ENTER to Return to Menu", screenWidth / 2 - MeasureText("Press ENTER to Return to Menu", 40) / 2, screenHeight / 2, 40, WHITE);
}
