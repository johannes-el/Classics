#pragma once
#include <raylib.h>

class Player {
  public:
    Player();
    Player(float x, float y);
    void move_player();
    void draw_player();
  private:
    Vector2 position;
};
