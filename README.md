# Ping Pong In Cpp VS AI
Description:
This is a simple implementation of the classic Pong game using the Raylib library. The game features a player paddle, a CPU paddle, and a ball. The player controls the paddle using the arrow keys to hit the ball towards the CPU paddle. The CPU paddle follows the ball's position to try to hit it back. The game keeps track of the scores and has three states: MENU, PLAYING, and GAME OVER. The game includes sound effects for collisions and scoring.

Key Features:
- **Game States**: MENU, PLAYING, and GAME OVER states.
- **Player and CPU Paddles**: Player paddle controlled by arrow keys, CPU paddle follows the ball.
- **Ball Movement**: Ball bounces off the paddles and walls.
- **Scoring**: Scores are updated when the ball passes the player or CPU paddle.
- **Sound Effects**: Collision and score sound effects.
- **Graphics**: Simple graphical representation with a central circle, middle dividing line, and rounded rectangles for paddles.

Function Breakdown:
1. **Ball Class**:
   - **draw**: Draws the ball on the screen.
   - **update**: Updates the ball's position, handles collisions with the walls, and checks for scoring conditions.
   - **reset**: Resets the ball's position and speed.

2. **Paddle Class**:
   - **limitMovement**: Ensures the paddle stays within the screen boundaries.
   - **draw**: Draws the paddle on the screen.
   - **update**: Updates the paddle's position based on user input.

3. **CpuPaddle Class** (inherits from Paddle):
   - **update**: Updates the CPU paddle's position based on the ball's position.

4. **Main Function**:
   - Initializes the game window and audio device.
   - Loads the sound effects and checks for errors.
   - Initializes the ball and paddles.
   - Contains the main game loop which handles game states, updates game elements, and draws game elements.
   - Unloads the sound effects and closes the audio device and game window.

5. **Function Declarations**:
   - **DrawGame**: Draws the game elements (ball, paddles, scores).
   - **UpdateGame**: Updates the game elements (ball, paddles).
   - **CheckCollisionCircleRec**: Checks for collisions between the ball and paddles.
   - **ResetScores**: Resets the scores to 0.
   - **DrawMenu**: Draws the menu screen.
   - **DrawGameOver**: Draws the game over screen.
