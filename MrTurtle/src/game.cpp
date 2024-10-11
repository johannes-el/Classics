#include "game.hpp"

typedef enum GAME_STATE {TITLE_SCREEN = 0, GAME, RUN} GAME_STATE;

// set initial game state to title_screen
static GAME_STATE current_state = TITLE_SCREEN;

struct Fish {
  bool active;
  bool deadly;
  float speed;
  Vector2 position;
  Texture2D texture;
};

static std::vector<Fish> fishes;

Game::Game() noexcept 
  : player(40, 100), water_speed(10.f)
{
  water = LoadTexture("./Assets/water.png");

  Fish fish1;
  fish1.texture = LoadTexture("./Assets/fish1Texture.png");
  fish1.active = true;
  fish1.deadly = true;
  fish1.position = {800, 100};
  fish1.speed = 100.f;
  // fish1.speed = GetRandomValue(100, 200);

  fishes.push_back(fish1);

  Fish fish2;
  fish2.texture = LoadTexture("./Assets/fish2Texture.png");
  fish2.active = false;
  fish2.deadly = false;
  fish2.position = {static_cast<float>(GetScreenWidth() + fish2.texture.width), 200};
  fish2.speed = 240;

  Fish fish3;
  fish3.texture = LoadTexture("./Assets/fish3Texture.png");
  fish2.active = false;
  fish2.deadly = true;
  fish2.position = {};
  fish2.speed = GetRandomValue(200, 300);

  Fish fish4;
  fish4.texture = LoadTexture("./Assetsfish4Texture.png");
  fish2.active = false;
  fish2.deadly = false;
  fish2.position = {};
  fish2.speed = GetRandomValue(150, 350);
}

void Game::unload_textures() {
  UnloadTexture(water);
  for (auto &fish : fishes) {
    UnloadTexture(fish.texture);
  }
}

void Game::run() {
  BeginDrawing();
  update();
  EndDrawing();
}

void Game::update() {
  switch(current_state) {
    case TITLE_SCREEN: display_title_screen(); break;
    case GAME: display_game(); break;
    case RUN: display_end_screen(); break;
  }
}

void Game::display_title_screen() {
  ClearBackground(RAYWHITE);
  float delta = GetFrameTime();
  DrawTextureRec(water, {water_one.x, 0, 1280, 200}, {0, 0}, WHITE);
  water_one.x += 200 * delta;

  DrawTextureRec(water, {water_two.x, 0, 1280, 200}, {0, 200}, WHITE);
  water_two.x += 150 * delta;

  DrawTextureRec(water, {water_three.x, 0, 1280, 200}, {0, 400}, WHITE);
  water_three.x += 220 * delta;

  DrawTextureRec(water, {water_four.x, 0, 1280, 200}, {0, 600}, WHITE);
  water_four.x += 100 * delta;
  player.draw_player();
  player.move_player();

  for (auto fish : fishes) {
    if (fish.active) {
      DrawTexturePro(fish.texture, {0, 0, -78, 50}, {fish.position.x, fish.position.y, 78, 50}, {0, 0}, 0, WHITE);
    }
    fish.position.x -= fish.speed * GetFrameTime();
  }
}

void Game::display_game() {
  ClearBackground(RAYWHITE);
}
void Game::display_end_screen() {
  ClearBackground(RAYWHITE);

}
