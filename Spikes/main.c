#include "raylib.h"
#include <stdlib.h>

#define MAX_SPIKES_RIGHT 10
#define MAX_SPIKES_LEFT  10

// ---------------------------------------------------------
// player structure definition
typedef struct Player {
  Rectangle rec;
} Player;


typedef struct Spike {
  bool active;
  float rotation;
  Rectangle rec;
  Color color;
} Spike;
// ---------------------------------------------------------

typedef enum GameStates {START, GAME} GameStates;

// ---------------------------------------------------------
// constant variables (local)
static const int screen_width = 800, screen_height = 800;
// ---------------------------------------------------------

// ---------------------------------------------------------
// game textures (local)
static Texture2D background_texture;
static Texture2D bird;
static Texture2D cake;
// ---------------------------------------------------------


// ---------------------------------------------------------
// game variables
static float horizontal_velocity = 150.f;
static float vertical_velocity   = 200.f;
static int left_score;
static int right_score;
static bool move_upward = false;
static GameStates current_state = START;

static Player player;
static Font font;
static Rectangle coin;
static Rectangle source_bird;
static Spike right_spikes[MAX_SPIKES_RIGHT];
static Spike left_spikes[MAX_SPIKES_LEFT];

static Color circle_color = (Color){179, 185, 209, 255};
static Color text_color   = (Color){139, 147, 175, 255};
static Color spikes_color = (Color){109, 117, 141, 255};

// ---------------------------------------------------------


// ---------------------------------------------------------
static void init();
static void start();
static void draw_start();
static void update();
static void draw_update();
static void unload_textures();
// ---------------------------------------------------------

int main() {

  SetConfigFlags(FLAG_MSAA_4X_HINT); // enable anti-aliasing

  InitWindow(screen_width, screen_height, "Moving Square");
  SetTargetFPS(60);
  init();

// main game loop
// ---------------------------------------------------------
  while (!WindowShouldClose()) {
    BeginDrawing();
    switch (current_state) {
      case START: start(); break;
      case GAME: update(); break;
      default: break;
    }
    EndDrawing();
  }
// ---------------------------------------------------------

  // cleanup
  unload_textures();
  CloseWindow();

  return 0;
}

// ------------------------------------------
// Game initialization:
// - texture loading
// - set positions        (player, obstacle)
// - set width and height (player, obstacle)
// ------------------------------------------
void init() {
  Image board = GenImageChecked(screen_width, screen_height, 40, 40, BLACK, (Color){10, 10, 10, 255});
  background_texture = LoadTextureFromImage(board);
  UnloadImage(board);

  font = LoadFont("./Assets/jupiter_crash.png");

  bird = LoadTexture("./Assets/bird.png");
  cake = LoadTexture("./Assets/cake.png");

  player.rec.width = player.rec.height = 64;
  player.rec.x = screen_width / 2.f - player.rec.width / 2.f;
  player.rec.y = screen_height / 2.f;

  for (int i = 0; i < MAX_SPIKES_LEFT; ++i) {
    left_spikes[i].active = false;
  }
  for (int i = 0; i < MAX_SPIKES_RIGHT; ++i) {
    right_spikes[i].active = false;
  }

  coin.x = GetRandomValue(20, screen_width - 20);
  coin.y = GetRandomValue(50, screen_height - 70);
  coin.width = coin.height = 20;

  source_bird.x = source_bird.y = 0;
  source_bird.width = source_bird.height = 32;
}

// Game De-Initialization
void unload_textures() {
  UnloadTexture(bird);
  UnloadTexture(background_texture);
  UnloadFont(font);
}


// Draw the starting screen.
void draw_start() {

  DrawCircle(screen_width / 2.f, screen_height / 2.f + 60, 150, circle_color);
  DrawTexturePro(bird, source_bird, player.rec, (Vector2){0, 0}, 0, WHITE);

  DrawTextEx(font, "DON'T TOUCH", (Vector2){190, 150}, 70, 20, text_color);
  DrawTextEx(font, "THE SPIKES", (Vector2){200, 230}, 70, 20, text_color);

  DrawRectangleRec((Rectangle){0, 0, screen_width, 50}, spikes_color);
  DrawRectangleRec((Rectangle){0, screen_height - 50, screen_width, 50}, spikes_color);

  // Draw the spikes on the top and on the bottom.
  for (int i = 20; i <= screen_width - 100; i += 50) {
    DrawRectanglePro((Rectangle){i + 50, 0, 60, 60}, (Vector2){0, 0}, 45, spikes_color);
    DrawRectanglePro((Rectangle){i + 50, screen_height - 80, 60, 60}, (Vector2){0, 0}, 45, spikes_color);
  }
}

// Handle starting screen logic:
// - player velocity
// - switch to Game State

void start() {

  ClearBackground((Color){218, 224, 234, 255});
  draw_start();

  player.rec.y -= vertical_velocity * GetFrameTime();
  vertical_velocity -= 10;
  if (vertical_velocity < -400) {
    vertical_velocity = 400.f;
  }
  if (IsKeyPressed(KEY_ENTER)) {
    current_state = GAME;
  }
}

// Handle update screen logic:
void update() {

  ClearBackground(RAYWHITE);
  draw_update();
  // Reset the game bird hits spikes.
  if (player.rec.y < 100 || player.rec.y + player.rec.height > screen_height - 100) {

    player.rec.x = screen_width / 2.f - player.rec.width / 2.f;
    player.rec.y = screen_height / 2.f;

    source_bird.width = abs(source_bird.width);
    horizontal_velocity = abs(horizontal_velocity);
    vertical_velocity = 200.f;

    source_bird.x = source_bird.y = 0;

    left_score = right_score = 0; // reset score
    current_state = START;
  }

  // Check for collision between player and spikes.
  // Mirror player sprite if wall is touched.
  if (player.rec.x <= 0 || player.rec.x >= screen_width - player.rec.width) {
    horizontal_velocity *= -1;
    source_bird.width = -source_bird.width;
  }

  if (IsKeyPressed(KEY_SPACE))
    vertical_velocity = 200.f;

  player.rec.y -= vertical_velocity * GetFrameTime();
  vertical_velocity -= 5;

  if (CheckCollisionRecs(player.rec, coin)) {
    coin.x = GetRandomValue(100, screen_width - 100);
    coin.y = GetRandomValue(250, screen_height - 300);
  }

  player.rec.x += horizontal_velocity * GetFrameTime();

  if (CheckCollisionRecs(player.rec, coin)) {
    if (right_score == 9) {
      right_score = 0;
      left_score += 1;
    }
    else {
      right_score++;
    }
  }
}

// Draw update screen
void draw_update() {


  // Draw background
  DrawTexture(background_texture, 0, 0, WHITE);
  DrawCircle(screen_width / 2.f, screen_height / 2.f, 70, (Color){179, 185, 209, 255});

  // Draw score
  DrawText(TextFormat("%d", left_score), screen_width / 2.f - 25, screen_height / 2.f - 25, 50, RAYWHITE);
  DrawText(TextFormat("%d", right_score), screen_width / 2.f + 5, screen_height / 2.f - 25, 50, RAYWHITE);

  // Draw bird and obstacle
  DrawTexturePro(bird, source_bird, player.rec, (Vector2){0, 0}, 0, WHITE);
  DrawTexture(cake, coin.x - cake.width / 2.f + 10, coin.y - 10, WHITE);

  // Draw spikes
  for (int i = 0; i <= screen_width; i += 40) {
    DrawRectanglePro((Rectangle){i, 50, 40, 40}, (Vector2){0, 0}, 45, spikes_color);
    DrawRectanglePro((Rectangle){i, screen_height - 100, 40, 40}, (Vector2){0, 0}, 45, spikes_color);
  }

  DrawRectangle(0, 0, screen_width, 80, spikes_color);
  DrawRectangle(0, screen_height - 80, screen_width, 100, spikes_color);
}
