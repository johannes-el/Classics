#include <raylib.h>
#include "Headers/player.h"
#include "Headers/screen.h"
#include "Headers/consts.h"

typedef enum States {
    START_SCREEN,
    PLAY
} States;

int main() {

  // --------------------------------------------------
  // Initialization
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
  InitAudioDevice();
  SetTargetFPS(120);
  init_rocks();
  init_mouse();
  // --------------------------------------------------

  // ------------------------------------------------------
  // load music
  Music startMusic = LoadMusicStream("./../res/Music/8-bit-arcade.mp3");
  Music playMusic = LoadMusicStream("./../res/Assets/Music/legends.mp3");
  // ------------------------------------------------------

  // start music playing
  PlayMusicStream(startMusic);
  PlayMusicStream(playMusic);

  float timePlayed = 0.f;

  Player player = init_player(); // initialize player object
  float speed = 400; // set player speed

  Texture2D bullet = LoadTexture("./../res/Assets/Airship.png"); // load bullet texture

  // set current window state
  States current_state = START_SCREEN;

  float *xPositions = init_star_x_positions();
  float *yPositions = init_star_y_positions();

  while (!WindowShouldClose()) {

    BeginDrawing(); // Setup canvas (framebuffer) to start drawing
    // -----------------------------------------------------------------------------------------------------------
    // Use state-machine based on current_state
    switch (current_state) {

      case START_SCREEN:
        // handle music
        // ------------------------------------------------------------------------
        UpdateMusicStream(startMusic);
        timePlayed = GetMusicTimePlayed(startMusic)/GetMusicTimeLength(startMusic);
        if (timePlayed > 1.0f) timePlayed = 1.0f;
        // ------------------------------------------------------------------------

        // check for mouse button click and change to game  or change to game when enter key has been pressed
        if (draw_start_screen() || IsKeyPressed(KEY_ENTER)) current_state = PLAY;
        break;

      case PLAY:
        // handle music
        // ----------------------------------------------------------------------
        UpdateMusicStream(playMusic);
        timePlayed = GetMusicTimePlayed(playMusic)/GetMusicTimeLength(playMusic);
        // ----------------------------------------------------------------------
        // DrawTexturePro(bullet, (Rectangle){0, 0, 53, 17}, (Rectangle){20, 20, 106, 34}, (Vector2){0, 0}, 0, WHITE);
        draw_game_screen(xPositions, yPositions);
        draw_player(&player);
        move_player(&player, speed);
        throw_rocks();

        break;
      default:
        break;
    }
    // -----------------------------------------------------------------------------------------------------------
    EndDrawing(); // End canvas drawing and swap buffers
  }
  // cleanup
  // ----------------------------
  UnloadTexture(bullet);
  destruct_player(&player);
  unload_rocks();
  unload_mouse();
  UnloadMusicStream(startMusic);
  CloseWindow();
  // ----------------------------
}
