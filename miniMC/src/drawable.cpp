#include "drawable.h"
#include <la.h>

Drawable::Drawable(OpenGLContext* context)
    : bufIdx(), bufPos(), bufNor(), bufCol(), bufUV(), bufAnim(), bufCombine(),
      idxBound(false), posBound(false), norBound(false), colBound(false),
      uvBound(false), animBound(false), combineBound(false),
      context(context)
{}

Drawable::~Drawable()
{}


void Drawable::destroy()
{
    context->glDeleteBuffers(1, &bufIdx);
    context->glDeleteBuffers(1, &bufPos);
    context->glDeleteBuffers(1, &bufNor);
    context->glDeleteBuffers(1, &bufCol);
    context->glDeleteBuffers(1, &bufUV);
    context->glDeleteBuffers(1, &bufAnim);
    context->glDeleteBuffers(1, &bufCombine);
    idxBound = false;
    posBound = false;
    norBound = false;
    colBound = false;
    uvBound = false;
    animBound = false;
    combineBound = false;
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return GL_TRIANGLES;
}

int Drawable::elemCount()
{
    return count;
}

void Drawable::generateIdx()
{
    idxBound = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    context->glGenBuffers(1, &bufIdx);
}

void Drawable::generatePos()
{
    posBound = true;
    // Create a VBO on our GPU and store its handle in bufPos
    context->glGenBuffers(1, &bufPos);
}

void Drawable::generateNor()
{
    norBound = true;
    // Create a VBO on our GPU and store its handle in bufNor
    context->glGenBuffers(1, &bufNor);
}

void Drawable::generateCol()
{
    colBound = true;
    // Create a VBO on our GPU and store its handle in bufCol
    context->glGenBuffers(1, &bufCol);
}

void Drawable::generateUV()
{
    uvBound = true;
    // Create a VBO on our GPU and store its handle in bufCol
    context->glGenBuffers(1, &bufUV);
}

void Drawable::generateAnim()
{
    animBound = true;
    context->glGenBuffers(1, &bufAnim);
}

void Drawable::generateCombine()
{
    combineBound = true;
    context->glGenBuffers(1, &bufCombine);
}

bool Drawable::bindIdx()
{
    if(idxBound) {
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    }
    return idxBound;
}

bool Drawable::bindPos()
{
    if(posBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    }
    return posBound;
}

bool Drawable::bindNor()
{
    if(norBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    }
    return norBound;
}

bool Drawable::bindCol()
{
    if(colBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    }
    return colBound;
}

bool Drawable::bindUV()
{
    if(uvBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);
    }
    return uvBound;
}

bool Drawable::bindAnim()
{
    if(animBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufAnim);
    }
    return animBound;
}

bool Drawable::bindCombine()
{
    if(combineBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCombine);
    }
    return combineBound;
}
