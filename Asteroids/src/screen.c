#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include "Headers/screen.h"

//TODO: flames in start screen

typedef struct Rock {
    float x;
    float y;
    float direction;
    float rotation;
    float rotationSpeed;
    Vector2 goal_position;
    float speed;
} Rock;


// hold x/y-Positions for stars
static float xPositions[STAR_NUMBER];
static float yPositions[STAR_NUMBER];

static Texture2D rockTexture;
static Rock rocks[10];


// Timer definitions and start and stop functions
// ----------------------------------------------
/* typedef struct Timer { */
/*     float Lifetime; */
/* } Timer; */

/* void start_timer(Timer *timer, float Lifetime) { */
/*     if (timer != NULL) */
/*         timer->Lifetime = Lifetime; */
/* } */

/* // update timer on every tick */
/* void update_timer(Timer *timer) { */
/*     if (timer != NULL && timer->Lifetime > 0) */
/*         timer->Lifetime -= GetFrameTime(); */
/* } */

/* // check if timer is finished */
/* bool timer_done(Timer *timer) { */
/*     if (timer != NULL) */
/*         return timer->Lifetime <= 0; */
/*     return false; */
/* } */
/* // ---------------------------------------------- */


void init_rocks() {
    rockTexture = LoadTexture("./../res/Assets/Rock.png");
    for (int i = 0; i < 10; ++i) {
        Rock rock;
        if (i < 3) {
            rock.x = GetRandomValue(0, SCREEN_WIDTH);
            rock.y = GetRandomValue(-20, -10);
            rock.goal_position.x = GetRandomValue(0, SCREEN_WIDTH);
            rock.goal_position.y = SCREEN_HEIGHT + GetRandomValue(10, 20);
        }
        if (i >= 3 && i < 6) {
            rock.x = GetRandomValue(-20, -10);
            rock.y = GetRandomValue(0, SCREEN_HEIGHT);
            rock.goal_position.x = SCREEN_WIDTH + GetRandomValue(10, 20);
            rock.goal_position.y = GetRandomValue(0, SCREEN_HEIGHT);
        }
        if (i >= 6) {
            rock.x = GetRandomValue(SCREEN_WIDTH + 10, SCREEN_WIDTH + 20);
            rock.y = GetRandomValue(0, SCREEN_HEIGHT);
            rock.goal_position.x = GetRandomValue(-20, -10);
            rock.goal_position.y = GetRandomValue(0, SCREEN_HEIGHT);
        }
        rock.rotation = 0;
        rock.rotationSpeed = GetRandomValue(1, 100);
        rock.speed = GetRandomValue(100, 200);
        rocks[i] = rock;
    }
}

// unload rocks texture
void unload_rocks() {
    UnloadTexture(rockTexture);
}

void throw_rocks() {
    for (int i = 0; i < 10; ++i) {
        DrawTexturePro(rockTexture, (Rectangle){0, 0, 32, 32}, (Rectangle){rocks[i].x, rocks[i].y, 64, 64}, (Vector2){16, 16}, rocks[i].rotation, WHITE);
        rocks[i].rotation += rocks[i].rotationSpeed / 50;
        rocks[i].x += 0.001 * sqrtf(pow((rocks[i].x - rocks[i].goal_position.x), 2) + pow((rocks[i].y - rocks[i].goal_position.y), 2));
        rocks[i].y += 0.001 * sqrtf(pow((rocks[i].x - rocks[i].goal_position.x), 2) + pow((rocks[i].y - rocks[i].goal_position.y), 2));
    }
}


static Texture2D mouse_texture;

void init_mouse() {
    mouse_texture = LoadTexture("./Resources/Assets/freezing.png");
}

static int currentFrame = 0;

static int framesCounter = 0;
static int framesSpeed = 8;
static int current_x_pos = 0;
static int current_y_pos = 0;

void mouse_anim() {

    float current_mouse_x = GetMouseX();
    float current_mouse_y = GetMouseY();

    framesCounter++;
    if (framesCounter >= (60 / framesSpeed)) {
        framesCounter = 0;
        currentFrame++;
        if (currentFrame > 8)
            currentFrame = 0;
        current_x_pos += 100;
        if (current_y_pos == 800 && current_x_pos == 500) {
            current_x_pos = 0;
            current_y_pos = 0;
        }
        if (current_x_pos % 1000 == 0) {
            current_x_pos = 0;
            current_y_pos += 100;
        }
        if (current_y_pos % 1000 == 0) {
            current_y_pos = 0;
        }
    }
    DrawTexturePro(
        mouse_texture, (Rectangle){current_x_pos, current_y_pos, 100, 100},
        (Rectangle){current_mouse_x, current_mouse_y, 100, 100},
        (Vector2){50, 50}, 0, WHITE);
}

void unload_mouse() {
    UnloadTexture(mouse_texture);
}

bool draw_start_screen() {
        ClearBackground(RAYWHITE);
        DrawRectangleGradientV(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE, DARKPURPLE);
        // -----------------------------------------------------
        // draw start interface
        DrawText("Asteroids", 500, 200, 30, BLACK);
        DrawText("Press [Enter] to start!", 300, 350, 50, BLACK);
        DrawLine(650, 400, 795, 400, BLACK);
        DrawLine(650, 400, 650, 450, BLACK);
        DrawLine(650, 450, 795, 450, BLACK);
        DrawLine(795, 400, 795, 450, BLACK);
        DrawText("... or press -> here <-", 300, 400, 50, BLACK);
        // -----------------------------------------------------

        float current_mouse_x = GetMouseX();
        float current_mouse_y = GetMouseY();

        mouse_anim();

        if (IsMouseButtonDown(0) && current_mouse_x >= 650 &&
            current_mouse_x <= 795 && current_mouse_y >= 400 &&
            current_mouse_y <= 450) {
            return true;
        }

        return false;
}

float* init_star_x_positions() {
    for (int i = 0; i < STAR_NUMBER; ++i) {
        xPositions[i] = GetRandomValue(0, SCREEN_WIDTH);
    }
    return xPositions;
}
float* init_star_y_positions() {
    for (int i = 0; i < STAR_NUMBER; ++i) {
        yPositions[i] = GetRandomValue(0, SCREEN_HEIGHT);
    }
    return yPositions;
}

void draw_game_screen(float *xPositions, float *yPositions) {
    ClearBackground((Color){0, 0, 50, 255});
    // skip 10 random stars
    int skip[10];
    for (int i = 0; i < 10; i++) {
        skip[i] = GetRandomValue(0, STAR_NUMBER);
    }
    for (int i = 0; i < STAR_NUMBER; ++i) {
        for (int j = 0; j < 10; j++) {
            if (skip[j] == i) break;
            DrawRectangle(xPositions[i], yPositions[i], 2, 2, YELLOW);
        }
    }
}

typedef struct Flame {
    Vector2 position;
    float size;
    float speed;
} Flame;

// static Flame flame[100];

void init_flames() {

}
