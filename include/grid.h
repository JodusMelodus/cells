#ifndef GRID_H
#define GRID_H

#include <stdlib.h>
#include <stdint.h>
#include <raylib.h>

typedef struct Grid
{
    uint16_t width, height;
    Color *buffer, *tempBuffer;
} Grid;

Grid *GridCreate(uint16_t width, uint16_t height);
int GridUpdate(Grid *grid);
void GridFree(Grid *grid);

#endif