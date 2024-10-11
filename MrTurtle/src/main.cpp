#include <iostream>
#include <raylib.h>
#include "game.hpp"

auto main() -> int {

  constexpr int screen_width = 1280;
  constexpr int screen_height = 800;

  // initialize window
  InitWindow(screen_width, screen_height, "Mr Turtle");
  SetTargetFPS(60);

  Game *game = new (std::nothrow) Game();

  while (!WindowShouldClose()) {
    game->run();
  }

  // cleanup
  game->unload_textures();
  delete game;
  CloseWindow();

  return 0;
}
