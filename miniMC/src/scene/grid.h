#ifndef GRID_H
#define GRID_H

#include "la.h"

#define MaxU 1024
#define MaxV 1024

class Grid
{
public:
    bool grid[MaxU*MaxV];
    Grid();
    void setValue(glm::ivec2 pos, bool value);
    bool getValue(glm::ivec2 pos);
    void setLineValue(glm::vec2 posFrom, glm::vec2 posTo, bool value, float width = 3.0f);
    void printAll();
};

#endif // GRID_H
