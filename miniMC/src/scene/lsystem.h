#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "la.h"
#include <vector>

#include "scene/grid.h"
#include "scene/turtle.h"

class LSystem
{
public:
    Grid grid;
    std::vector<Turtle*> turtleV;
    void parse();
    void generate(int count = 128, glm::vec2 origin = glm::vec2(0,0), glm::vec2 dir = glm::vec2(1, 1));
    bool get(glm::ivec2 pos);
    void destroy();
    void printAll();
    LSystem();
    ~LSystem();
};

#endif // LSYSTEM_H
