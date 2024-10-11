#ifndef PLAYER_H_
#define PLAYER_H_
#include <raylib.h>
#include "consts.h"

typedef struct Player {
    float x;
    float y;
    Texture2D texture;
    float rotation;
} Player;

Player init_player();
void draw_player(const Player *player);
void move_player(Player *const player, float speed);
void destruct_player(const Player *player);

#endif // PLAYER_H_
