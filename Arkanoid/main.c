/* ------------------------------------*/
/* Copyright (c) 2024, Johannes Elsing */
/* ------------------------------------*/

#include "raylib.h"
#include <math.h>

#define COLUMNS    20
#define ROWS       5

// -------------------------------------------------------------------------
// Types and structures definition
// -------------------------------------------------------------------------
// Player structure
typedef struct Paddle
{
  Rectangle rec;
  float speed;
  int life;
} Paddle;

// Brick structure
typedef struct Brick
{
  Vector2 position;
  bool active;
} Brick;

// Ball structure
typedef struct Ball
{
  Vector2 position;
  Color color;
  float size;
  float horizontal_speed;
  float vertical_speed;
} Ball;
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
// Global Variables Declarations
// -------------------------------------------------------------------------

static const int screen_width  = 1200;
static const int screen_height = 800;
static int lives = 5;

static Paddle paddle;
static Ball   ball;
static Brick  bricks[COLUMNS][ROWS];

static const float brick_size = 60;

static Color brick_color[2] = {GRAY, DARKGRAY};
static Color current_color;

static bool toggle_color = false;
static bool ball_move    = false;
static bool collides     = false;

// -------------------------------------------------------------------------
// Module Function Forward Declarations
// -------------------------------------------------------------------------

static void init(void);        // Initialize the game
static void draw_player(void); // Draw ball and paddle
static void draw_bricks(void); // Draw bricks
static void update(void);      // Update the game logic

// -------------------------------------------------------------------------
// Program main entry point.
// -------------------------------------------------------------------------
int main()
{
  // ------------------------------------------------------------------------
  // Initialize game
  InitWindow(screen_width, screen_height, "Arkanoid");
  SetTargetFPS(60);
  init();
  // ------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose())
  {

    update();

    BeginDrawing();

    draw_player();
    draw_bricks();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void init()
{
  // Initialize the paddle (player)
  // --------------------------------------------------------------------
  paddle.rec.width  = 150;
  paddle.rec.height = 30;

  paddle.rec.x = screen_width / 2.f - paddle.rec.width / 2.f;
  paddle.rec.y = screen_height / 2.f - paddle.rec.height / 2.f + 300;

  paddle.speed = 250.f;
  paddle.life  = 5;
  // --------------------------------------------------------------------

  ball.size             = 15.f;
  ball.vertical_speed   = -200;
  ball.horizontal_speed = 0;

  ball.position = (Vector2){
    screen_width / 2.f,
    screen_height / 2.f + 220,
  };
  ball.color = RED;

  // Initialize bricks.
  for (int i = 0; i < COLUMNS; ++i)
  {
    for (int j = 0; j < ROWS; ++j)
    {
      bricks[i][j].active = true; // Activate all bricks.
      bricks[i][j].position = (Vector2){i * brick_size, j * brick_size};
    }
  }
}

void draw_player()
{
  // Draw the paddle.
  DrawRectangleRec(paddle.rec, BLACK);
  // Draw the ball.
  DrawCircleV(ball.position, ball.size, ball.color);
}

void draw_bricks()
{
  ClearBackground(WHITE); 

  // Draw colored bricks if they are active (active by default)

  for (int i = 0; i < COLUMNS; ++i)
  {
    for (int j = 0; j < ROWS; ++j)
    {
      current_color = toggle_color ? brick_color[0] : brick_color[1];
      toggle_color = !toggle_color;

      if (bricks[i][j].active) {
        DrawRectangleRec((Rectangle)
          {
            bricks[i][j].position.x,
            bricks[i][j].position.y,
            brick_size, brick_size
          },
          current_color
        );
      }
    }
  }
  for (int i = 0; i < lives; ++i)
  {
    DrawRectangleRec((Rectangle){15 + 50 * i, screen_height - 40, 40, 15}, LIGHTGRAY);
  }
}

void update()
{
  if (lives == 0)
  {
    if (IsKeyPressed(KEY_ENTER))
    {
      lives = 5;
    }
    DrawText("You died!", 500, 400, 40, BLACK);
    DrawText("Press [ENTER] to play again!", 430, screen_height - 50, 25, BLACK);
    return;
  }
  // ----------------------------------------------------------------------
  // Limit paddle (player) so it can't get out of bounds
  if (paddle.rec.x <= 0)
  {
    paddle.rec.x = 0;
  }
  if (paddle.rec.x + paddle.rec.width >= screen_width)
  {
    paddle.rec.x = screen_width - paddle.rec.width;
  }
  // ----------------------------------------------------------------------

  float delta = GetFrameTime();

  if (IsKeyPressed(KEY_SPACE)) ball_move = true;

  if (ball_move)
  {
    // ----------------------------------------------------------------------------
    // Move paddle
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      paddle.rec.x -= paddle.speed * delta;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      paddle.rec.x += paddle.speed * delta;
    // ----------------------------------------------------------------------------

    // Flip vertical/horizontal speed if collision between paddle and ball occurs
    if (CheckCollisionCircleRec(ball.position, ball.size, paddle.rec))
    {
      ball.vertical_speed *= -1;
      ball.horizontal_speed *= -1;
    }

    // Update ball x, y coordinates
    ball.position.x += ball.horizontal_speed * delta;
    ball.position.y += ball.vertical_speed * delta;

    // Reverse horizontal_speed of ball when left or right wall has been hit
    if (ball.position.x <= ball.size || ball.position.x >= screen_width - ball.size)
      ball.horizontal_speed *= -1;

    else if (ball.position.y <= ball.size)
      ball.vertical_speed *= -1;

    else if (ball.position.y >= screen_height - ball.size)
    {
      lives--;
      init();
      ball_move = false;
    }

    if (collides)
    {
      collides = false;
    }
    for (int i = 0; i < COLUMNS; ++i) {
      for (int j = 0; j < ROWS; ++j) {

	if (bricks[i][j].active && 
            CheckCollisionCircleRec(ball.position, ball.size, (Rectangle){bricks[i][j].position.x, bricks[i][j].position.y, brick_size, brick_size})) {

            bricks[i][j].active = false;  // Deactivate the brick
            ball.vertical_speed *= -1;    // Reverse vertical speed

            // Determine where the ball is relative to the brick to adjust its position
            float ballBottom = ball.position.y + ball.size; 
            float ballTop = ball.position.y - ball.size;
            float brickBottom = bricks[i][j].position.y + brick_size;
            float brickTop = bricks[i][j].position.y;

            // Check if the ball is approaching from the top or bottom
            if (ballBottom >= brickTop && ballTop < brickTop) {
                ball.position.y = brickTop - ball.size; // Place ball above the brick
            } else if (ballTop <= brickBottom && ballBottom > brickBottom) {
                ball.position.y = brickBottom + ball.size; // Place ball below the brick
            }
        }
      }
    }
  }
}
