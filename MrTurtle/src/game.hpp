#pragma once
#include <raylib.h>
#include <vector>
#include "player.hpp"

class Game {
  public:
    Game() noexcept;
    void run();
    void update();
    void unload_textures();
    void display_title_screen();
    void display_game();
    void display_end_screen();

  private:
    Player player;
    Texture2D water;
    float water_speed;
    Vector2 water_one;
    Vector2 water_two;
    Vector2 water_three;
    Vector2 water_four;
};
