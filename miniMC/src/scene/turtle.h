#ifndef TURTLE_H
#define TURTLE_H

#include "la.h"

class Turtle
{
public:
    glm::vec2 pos;
    glm::vec2 dir;
    float speed;
    float life;
    Turtle(glm::vec2 _pos, glm::vec2 _dir, float _speed = 1, float _life = 20);
};

#endif // TURTLE_H
