#include "player.hpp"

Player::Player(float x, float y) {
  this->position.x = x;
  this->position.y = y;
}

void Player::move_player() {
  if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
    // keep player in confined space
    if (position.y >= 200) {
      position.y -= 200;
    }
  }
  if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
    // keep player in confined space
    if (position.y <= 600) {
      position.y += 200;
    }
  }
}

void Player::draw_player() {
  DrawCircle(position.x, position.y, 10, RED);
}
