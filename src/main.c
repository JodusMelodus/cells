#include <stdio.h>
#include <raylib.h>

#include "grid.h"

#define WIDTH 1000
#define HEIGHT 500

int main(int argc, char const *argv[])
{
    InitWindow(WIDTH, HEIGHT, "Cells");
    SetTargetFPS(60);

    Grid *grid = GridCreate(WIDTH, HEIGHT);

    Image img = {
        .data = grid->buffer,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .height = HEIGHT,
        .width = WIDTH,
        .mipmaps = 1};

    Texture2D texture = LoadTextureFromImage(img);

    grid->buffer[2 + 1 * WIDTH] = WHITE;
    grid->buffer[0 + 2 * WIDTH] = WHITE;
    grid->buffer[2 + 2 * WIDTH] = WHITE;
    grid->buffer[1 + 3 * WIDTH] = WHITE;
    grid->buffer[2 + 3 * WIDTH] = WHITE;

    while (!WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))
    {
        GridUpdate(grid);
        UpdateTexture(texture, grid->buffer);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();
    GridFree(grid);
    return 0;
}