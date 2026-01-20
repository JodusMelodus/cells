#include <stdio.h>
#include <raylib.h>
#include <time.h>

#include "grid.h"

const int WIDTH = 1000;
const int HEIGHT = 500;

int main(int argc, char const *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Cells");
    SetTargetFPS(60);

    Grid *grid = GridCreate(WIDTH, HEIGHT);
    int currentWidth = WIDTH;
    int currentHeight = HEIGHT;

    Image img = {
        .data = grid->buffer,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .height = HEIGHT,
        .width = WIDTH,
        .mipmaps = 1};

    Texture2D texture = LoadTextureFromImage(img);
    Vector2 mouse = GetMousePosition();
    bool paused = true;

    while (!WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))
    {
        int newWidth = GetScreenWidth();
        int newHeight = GetScreenHeight();

        if (newWidth != currentWidth || newHeight != currentHeight)
        {
            currentWidth = newWidth;
            currentHeight = newHeight;
            
            
            UnloadTexture(texture);
            GridFree(grid);
            
            grid = GridCreate(currentWidth, currentHeight);
            srand((unsigned int)time(NULL));

            for (size_t y = 0; y < currentHeight; y++)
            {
                for (size_t x = 0; x < currentWidth; x++)
                {
                    if ((float)rand() / RAND_MAX < 0.3)
                    {
                        grid->buffer[x + y * currentWidth] = WHITE;
                    }
                }
            }
            
            img.data = grid->buffer;
            img.width = currentWidth;
            img.height = currentHeight;

            texture = LoadTextureFromImage(img);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            mouse = GetMousePosition();
            int x = (int)mouse.x;
            int y = (int)mouse.y;
            if (x >= 0 && x < currentWidth && y >= 0 && y < currentHeight)
                grid->buffer[x + y * currentWidth] = ColorIsEqual(grid->buffer[x + y * currentWidth], WHITE) ? BLACK : WHITE;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            paused = !paused;
            printf("%s\n", paused ? "Paused" : "Play");
        }

        if (!paused)
            GridUpdate(grid);

        UpdateTexture(texture, grid->buffer);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();
    UnloadTexture(texture);
    GridFree(grid);
    return 0;
}