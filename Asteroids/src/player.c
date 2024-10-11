#include <raylib.h>
#include <math.h>
#include "Headers/player.h"

Player init_player() {
    Player player;
    player.x = SCREEN_WIDTH / 2.f;
    player.y = SCREEN_HEIGHT / 2.f + 300;
    player.texture = LoadTexture("./../res/Assets/Player.png");
    player.rotation = 0;
    return player;
}

void destruct_player(const Player *player) {
    UnloadTexture(player->texture);
}

void draw_player(const Player *player) {
    DrawTexturePro(player->texture, (Rectangle){0, 0, 56, 61}, (Rectangle){player->x - 56, player->y - 61, 112, 122}, (Vector2){32, 32}, player->rotation, WHITE);
}

void move_player(Player *const player, float speed) {

    float frame_time = GetFrameTime();
    float distance = speed * frame_time;

    if (((IsKeyDown(KEY_A) != IsKeyDown(KEY_D)) &&
         (IsKeyDown(KEY_W) || IsKeyDown(KEY_S))) ||
        ((IsKeyDown(KEY_W) != IsKeyDown(KEY_S)) &&
         (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)))) {
        distance *= 1 / sqrtf(2);
    }
    if (IsKeyDown(KEY_A)) {
        player->x -= distance;
        player->rotation -= 0.8;
    }
    if (IsKeyDown(KEY_S)) {
        player->y += distance;
    }
    if (IsKeyDown(KEY_D)) {
         player->x += distance;
        player->rotation += 0.8;
    }
    if (IsKeyDown(KEY_W)) {
        player->y -= distance;
    }
}
