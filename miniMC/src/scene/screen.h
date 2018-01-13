#ifndef SCREEN_H
#define SCREEN_H

#include "drawable.h"

class Screen : public Drawable
{
public:
    Screen(OpenGLContext *context);
    void create();
    ~Screen();
};

#endif // SCREEN_H
