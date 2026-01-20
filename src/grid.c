#include <omp.h>
#include "grid.h"

Grid *GridCreate(uint16_t width, uint16_t height)
{
    Grid *grid = malloc(sizeof(Grid));
    if (!grid)
    {
        perror("Failed to allocate memory for grid");
        return NULL;
    }

    grid->tempBuffer = calloc(width * height, sizeof(Color));
    if (!grid->tempBuffer)
    {
        perror("Failed to allocate memory for temporary buffer");
        free(grid);
        return NULL;
    }
    grid->buffer = calloc(width * height, sizeof(Color));
    if (!grid->buffer)
    {
        perror("Failed to allocate memory for buffer");
        free(grid->tempBuffer);
        free(grid);
        return NULL;
    }

    grid->height = height;
    grid->width = width;
    return grid;
}

int GridUpdate(Grid *grid)
{
    uint16_t w = grid->width;
    uint16_t h = grid->height;

    for (uint16_t y = 1; y < h - 1; y++)
    {
        for (uint16_t x = 1; x < w - 1; x++)
        {
            uint8_t sum = 0;

            for (int8_t dy = -1; dy <= 1; dy++)
            {
                for (int8_t dx = -1; dx <= 1; dx++)
                {
                    if (dy == 0 && dx == 0)
                        continue;

                    uint16_t a = y + dy;
                    uint16_t b = x + dx;

                    if (ColorIsEqual(grid->buffer[a * w + b], WHITE))
                        sum++;
                }
            }

            int alive = ColorIsEqual(grid->buffer[y * w + x], WHITE);

            /* Conway's rules:
               - Any live cell with two or three live neighbours survives.
               - Any dead cell with exactly three live neighbours becomes a live cell.
               - All other live cells die in the next generation. */
            if (alive)
            {
                if (sum == 2 || sum == 3)
                    grid->tempBuffer[y * w + x] = WHITE;
                else
                    grid->tempBuffer[y * w + x] = BLACK;
            }
            else
            {
                if (sum == 3)
                    grid->tempBuffer[y * w + x] = WHITE;
                else
                    grid->tempBuffer[y * w + x] = BLACK;
            }
        }
    }

    /* clear borders to BLACK to avoid carrying previous garbage */
    for (uint16_t x = 0; x < w; x++)
    {
        grid->tempBuffer[0 * w + x] = BLACK;
        grid->tempBuffer[(h - 1) * w + x] = BLACK;
    }
    for (uint16_t y = 0; y < h; y++)
    {
        grid->tempBuffer[y * w + 0] = BLACK;
        grid->tempBuffer[y * w + (w - 1)] = BLACK;
    }

    /* swap buffers instead of aliasing them to avoid losing the original
        buffer pointer and to prevent double-free or incorrect memory use */
    Color *tmp = grid->buffer;
    grid->buffer = grid->tempBuffer;
    grid->tempBuffer = tmp;

    return 0;
}

void GridFree(Grid *grid)
{
    if (grid)
    {
        free(grid->buffer);
        free(grid->tempBuffer);
        free(grid);
    }
}
