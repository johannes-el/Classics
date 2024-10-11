/*
 * -----------------------------------------------------------------------
 * Copyright (C) 2024 Johannes E.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * -----------------------------------------------------------------------
*/

// Player sprites from:
// https://secrethideout.itch.io/team-wars-platformer-battle

// Firework sprites from:
// https://nyknck.itch.io/firework-pixel

// Music Kim Lightyear - Legends from:
// https://pixabay.com/music/search/8%20bit%20game/

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENEMIES 200
#define MAX_SHOTS   50
#define MAX_ENERGY  5

//------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------
// Player structure
typedef struct Player {
  Rectangle rec;
  Vector2 speed;
} Player;

// Enemy structure
typedef struct Enemy {
  Rectangle rec;
  float speed;
  bool active;
  Color color;
} Enemy;

// Shot structure
typedef struct Shot {
  Rectangle bullet;
  bool active;
  Color color;
} Shot;

// Energy structure
typedef struct Energy {
  bool active;
  Rectangle rec;
  Texture2D texture;
} Energy;

typedef struct Timer {
  float Lifetime;
} Timer;
//-----------------------------------------------------


//----------------------------------------------------
// Global variables declaration
//----------------------------------------------------
static const int screen_width  = 1200;
static const int screen_height = 800;

static int frames_counter          = 0;
static int animation_frame_counter = 0;
static int score                   = 0;
static bool god_mode     = false;
static bool initialized  = false;

static Texture2D idle;
static Texture2D run;
static Texture2D current_texture;
static Texture2D energy_texture;

// --------------------------
// textures for firework anim
static Texture2D firework_1;
static Texture2D firework_2;
static Texture2D firework_3;
static Texture2D firework_4;
static Texture2D firework_5;
static Texture2D firework_6;
// --------------------------

static float bullet_speed = 600.f;
static Color bullet_color;
static Color player_color;
static int texture_pos = {0};

static Player    player;
static Shot      shots[MAX_SHOTS];
static Enemy     enemies[MAX_ENEMIES];
static Energy    energy[MAX_ENERGY];
static Font      font;
static Music     intro_music;
static Timer     timer;
static Timer     god_mode_timer;

//---------------------------------------------------------------------


//---------------------------------------------------------------------
// Module Functions Declaration (local)
//---------------------------------------------------------------------
static void init_game();
static void draw_title_screen();
static void draw_gameplay_screen();
static void update();
static void draw_restart_screen();
static void draw_end_screen();
static void update_draw_frame();
static void cleanup();

static void set_timer(float Lifetimer, Timer *timer);
static void update_timer(Timer *timer);
static bool timer_down(Timer *timer);
//---------------------------------------------------------------------


typedef enum GameScreen {TITLE = 0, GAMEPLAY, RESTART, END} GameScreen;
GameScreen current_screen = TITLE;

int main() {

  // game setup
  // ----------------------------------------------------------------------
  InitWindow(screen_width, screen_height, "Space Invaders");
  InitAudioDevice();

  energy_texture = LoadTexture("./Assets/Sprites/Energy.png");
  intro_music =    LoadMusicStream("./Assets/Music/lightyear_legends.mp3");
  idle =           LoadTexture("./Assets/Sprites/Gunner_Blue_Idle.png");
  run =            LoadTexture("./Assets/Sprites/Gunner_Blue_Run.png");
  
  firework_1 = LoadTexture("./Assets/Sprites/yellow/1.png");
  firework_2 = LoadTexture("./Assets/Sprites/yellow/2.png");
  firework_3 = LoadTexture("./Assets/Sprites/yellow/3.png");
  firework_4 = LoadTexture("./Assets/Sprites/yellow/4.png");
  firework_5 = LoadTexture("./Assets/Sprites/yellow/5.png");
  firework_6 = LoadTexture("./Assets/Sprites/yellow/6.png");

  init_game();
  // ----------------------------------------------------------------------

  SetTargetFPS(60);

  font = LoadFont("./Assets/fonts/jupiter_crash.png");

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    update_draw_frame();
    // UpdateMusicStream(intro_music);
    // PlayMusicStream(intro_music);
  }
  
  // cleanup
  // -----------------------------
  cleanup();
  CloseAudioDevice();
  CloseWindow();
  // -----------------------------

  return 0;
}

void update_draw_frame() {
  BeginDrawing();

  // implementation of a state machine
  //--------------------------------------------------------
  switch (current_screen) {
    case TITLE: draw_title_screen(); break;
    case GAMEPLAY: draw_gameplay_screen(); update(); break;
    case RESTART: draw_restart_screen(); break;
    case END: draw_end_screen(); break;
    default: break;
  }
  //--------------------------------------------------------

  EndDrawing();
}

void draw_title_screen() {

  ClearBackground(RAYWHITE);
  DrawTextEx(font, "Space Invaders", (Vector2){320, 300}, 100, 10, RED);

  frames_counter++; // increment frame count

  if (frames_counter < 40)
    DrawText("PRESS [ENTER] to START", GetScreenWidth() * 0.3 + 30, GetScreenHeight() * 0.6, 30, GRAY);

  else if (frames_counter < 70) {
    ; // 30 frame wait time until text gets displayed again
  }
  else {
    frames_counter = 0;
  }

  // change to gameplay screen if enter key is pressed
  if (IsKeyPressed(KEY_ENTER))
    current_screen = GAMEPLAY;
}

void init_game() {

  bullet_color = RED;
  god_mode = false;
  player_color = WHITE;
  // Initialize enemies
  //--------------------------------------------------------------------------------
  for (int i = 0; i < MAX_ENEMIES; ++i) {
    enemies[i].rec.width = enemies[i].rec.height = 10;
    enemies[i].rec.x     = GetRandomValue(screen_width, screen_width + 1500);
    enemies[i].rec.y     = GetRandomValue(0, screen_height - enemies[i].rec.height);
    enemies[i].color     = RED;
    enemies[i].speed     = GetRandomValue(150, 200);
    enemies[i].active    = true;
  }
  //--------------------------------------------------------------------------------
  // Initialize player
  player.rec.x      = 80;
  player.rec.y      = 150;
  player.rec.width  = 25;
  player.rec.height = 50;
  player.speed.x    = player.speed.y = 350.f;
  // --------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  // Initialize shots
  for (int i = 0; i < MAX_SHOTS; ++i) {
    shots[i].color = RED;
    shots[i].active = false;
    shots[i].bullet.width = 10;
    shots[i].bullet.height = 5;
  }
  //---------------------------------------------------------------------------

  set_timer(6.f, &god_mode_timer);
  set_timer(10.f, &timer);

  // initialize energy
  for (int i = 0; i < MAX_ENERGY; ++i) {
    energy[i].texture = energy_texture;
    energy[i].active = false;
    energy[i].rec.x = GetRandomValue(10, screen_width - 200);
    energy[i].rec.y = GetRandomValue(20, screen_height - 20);
    energy[i].rec.width = energy[i].rec.height = 32;
  }

  // set initial animation
  current_texture = idle;
}

void draw_gameplay_screen() {

  if (!god_mode) {
    ClearBackground(RAYWHITE);
  }
  else {
    ClearBackground(RED);
  }

  DrawText(TextFormat("Current score: %d < 1000", score), 20, 20, 40, BLACK);

  // draw active shots
  for (int i = 0; i < MAX_SHOTS; ++i) {
    if (shots[i].active)
      DrawRectangleRec(shots[i].bullet, bullet_color);
  }

  // draw enemies
  for (int i = 0; i < MAX_ENEMIES; ++i) {
    if (enemies[i].active)
      DrawRectangleRec(enemies[i].rec, GRAY);
  }

  // draw player
  DrawTexturePro(
      current_texture, 
      (Rectangle){texture_pos, 0, 48, 48},
      (Rectangle){player.rec.x - player.rec.width + 8, player.rec.y - player.rec.height + 43, 70, 70},
      (Vector2){0, 0}, 0, player_color
  );

  // Draw current energy
  for (int i = 0; i < MAX_ENERGY; ++i) {
    if (energy[i].active) {
      DrawTexture(energy[i].texture, energy[i].rec.x, energy[i].rec.y, WHITE);
    }
  }
}

void update() {

  update_timer(&timer);

  // spawn energy if timer elapsed
  if (timer_down(&timer)) {

    set_timer(10.f, &timer);

    for (int i = 0; i < MAX_ENERGY; ++i) {
      if (!energy[i].active) {
        energy[i].rec.x = GetRandomValue(10, screen_width - 200);
        energy[i].rec.y = GetRandomValue(20, screen_height - 70);
        energy[i].active = true;
        break;
      }
    }
  }

  update_timer(&god_mode_timer);

  if (timer_down(&god_mode_timer)) {
    set_timer(6.f, &god_mode_timer);
    god_mode = false;
  }
  else if (god_mode && !timer_down(&god_mode_timer)) {
    bullet_color = BLACK;
    player_color = (Color){0, 0, 0, 30};
  }
  else {
    bullet_color = RED;
    player_color = WHITE;
  }

  float delta = GetFrameTime();

  bool key_a = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
  bool key_d = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
  bool key_w = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
  bool key_s = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);

  // -----------------------------------------------------------------------------------------------------
  // move player based on input
  if (key_a) player.rec.x -= player.speed.x * delta;
  if (key_d) player.rec.x += player.speed.x * delta;
  if (key_w) player.rec.y -= player.speed.x * delta;
  if (key_s) player.rec.y += player.speed.x * delta;
  // -----------------------------------------------------------------------------------------------------

  if (score >= 1000) current_screen = END;

  // -----------------------------------------------------------------------------------------------------
  // handle player animation
  if (!key_a && !key_d)
    current_texture = idle;
  else
    current_texture = run;

  animation_frame_counter += 1;

  if (animation_frame_counter % 5 == 0) {

    if (texture_pos != 240)
        texture_pos += 48;
    else
        texture_pos = 0;

      animation_frame_counter = 0;
  }

  // update enemies
  for (int i = 0; i < MAX_ENEMIES; ++i) {
    enemies[i].rec.x -= enemies[i].speed * GetFrameTime();

    if (!enemies[i].active || enemies[i].rec.x <= 0 - enemies[i].rec.width) {
      enemies[i].active = true;
      enemies[i].rec.x = screen_width;
      enemies[i].rec.y = GetRandomValue(0, screen_height - enemies[i].rec.height);
    }

    for (int j = 0; j < MAX_SHOTS; ++j) {
      if (CheckCollisionRecs(enemies[i].rec, shots[j].bullet) && enemies[i].active && shots[j].active) {
        enemies[i].active = false;
        score += 1;
      }
    }
  }
  // -----------------------------------------------------------------------------------------------------

  // Wall blocking
  // ------------------------------------------------------------------------------------------------------
  if (player.rec.x <= 0) player.rec.x = 0;
  if (player.rec.x + player.rec.width >= screen_width) player.rec.x = screen_width - player.rec.width - 10;
  if (player.rec.y <= 0) player.rec.y = 0;
  if (player.rec.y + player.rec.height >= screen_height) player.rec.y = screen_height - player.rec.height;
  // ------------------------------------------------------------------------------------------------------

  // player collision
  if (!god_mode) {
    for (int i = 0; i < MAX_ENEMIES; ++i) {
      if (CheckCollisionRecs(player.rec, enemies[i].rec) && enemies[i].active)
        current_screen = RESTART;
    }
  }

  // energy - player collision
  for (int i = 0; i < MAX_ENERGY; ++i) {
    if (energy[i].active && CheckCollisionRecs(energy[i].rec, player.rec)) {
      energy[i].active = false;
      god_mode = true;
    }
  }

  // ------------------------------------------------------------------------------------------------------
  // bullet related actions
  for (int i = 0; i < MAX_SHOTS; ++i) {
    // deactivate bullets that are out of scope
    if (shots[i].bullet.x >= screen_width + shots[i].bullet.width) {
      shots[i].active = false;
    }

    // move bullets by constant speed
    if (shots[i].active) {
      shots[i].bullet.x += bullet_speed * GetFrameTime();
    }
  }

  // spawn a new bullet is enter key is pressed in 2-frame interval
  if (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (frames_counter <= 2) {
      (frames_counter)++;
    }
    else {
      frames_counter = 0;
      for (int i = 0; i < MAX_SHOTS; ++i) {
        if (shots[i].active) {
          continue;
        }
        else {
          // activate shot and set y position
          shots[i].bullet.x = player.rec.x + 40;
          shots[i].bullet.y = player.rec.y + player.rec.height - 27;
          shots[i].active = true;
          break;
        }
      }
    }
  }
  // ------------------------------------------------------------------------------------------------------
}

void draw_restart_screen() {

  ClearBackground(DARKGRAY);
  DrawText("Ready to retry?", 320, 220, 60, GRAY);
  DrawText("Press enter to play again", 255, screen_height / 2, 50, GRAY);

  if (IsKeyPressed(KEY_ENTER)) {
    initialized = true; // Reset initialized flag when restarting
  }
  if (initialized) {
    score = 0;
    initialized = false;

    init_game();

    for (int i = 0; i < MAX_ENEMIES; ++i) {
      enemies[i].active = false;
    }
    for (int i = 0; i < MAX_SHOTS; ++i) {
      shots[i].active = false;
    }
    player.rec.x = player.rec.y = 100;
    current_screen = GAMEPLAY;
  }
}

void draw_fire_works() {
  if (frames_counter < 10) {
    DrawTexture(firework_1, 200, 200, WHITE);
    DrawTexture(firework_1, 700, 200, WHITE);
  }
  else if (frames_counter < 20 && frames_counter <= 30) {
    DrawTexture(firework_2, 200, 200, WHITE);
    DrawTexture(firework_2, 700, 200, WHITE);
  }
  else if (frames_counter < 30 && frames_counter <= 40) {
    DrawTexture(firework_3, 200, 200, WHITE);
    DrawTexture(firework_3, 700, 200, WHITE);
  }
  else if (frames_counter < 40 && frames_counter <= 50) {
    DrawTexture(firework_4, 200, 200, WHITE);
    DrawTexture(firework_4, 700, 200, WHITE);
  }
  else if (frames_counter < 50 && frames_counter <= 60) {
    DrawTexture(firework_5, 200, 200, WHITE);
    DrawTexture(firework_5, 700, 200, WHITE);
  }
  else if (frames_counter < 60 && frames_counter <= 70) {
    DrawTexture(firework_6, 200, 200, WHITE);
    DrawTexture(firework_6, 700, 200, WHITE);
  }
  frames_counter++;
  // reset frame counter to see animation periodically
  if (frames_counter > 100) frames_counter = 0;
}

void draw_end_screen() {
  ClearBackground(RAYWHITE);
  DrawText("You win!", 500, 300, 40, BLACK);
  draw_fire_works();
}


// -----------------------------------------------------------------
// timer logic
void set_timer(float lifetime, Timer *timer) {
  if (timer == NULL) {
    printf("Unable to dereference null-ptr. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  timer->Lifetime = lifetime;
}
void update_timer(Timer *timer) {
  if (timer == NULL) {
    printf("Unable to dereference null-ptr. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  timer->Lifetime -= GetFrameTime();
}
bool timer_down(Timer *timer) {
  if (timer == NULL) {
    printf("Unable to dereference null-ptr. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  return timer->Lifetime <= 0;
}
// -----------------------------------------------------------------

void cleanup() {
  UnloadFont(font);
  UnloadTexture(idle);
  UnloadTexture(run);
  UnloadTexture(energy_texture);
  UnloadTexture(current_texture);
  
  UnloadTexture(firework_1);
  UnloadTexture(firework_2);
  UnloadTexture(firework_3);
  UnloadTexture(firework_4);
  UnloadTexture(firework_5);
  UnloadTexture(firework_6);

  UnloadMusicStream(intro_music);
}
