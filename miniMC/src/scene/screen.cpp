#include "screen.h"

Screen::Screen(OpenGLContext* context) : Drawable(context)
{

}

Screen::~Screen()
{
    destroy();
}

void Screen::create()
{
    destroy();

    glm::vec4 pos[4] = { glm::vec4(-1,-1,0.009,1),
                         glm::vec4(1,-1,0.009,1),
                         glm::vec4(1,1,0.009,1),
                         glm::vec4(-1,1,0.009,1) };
    glm::vec4 col[4] = { glm::vec4(0,0,0.7,0.5),
                         glm::vec4(0,0,0.7,0.5),
                         glm::vec4(0,0,0.7,0.5),
                         glm::vec4(0,0,0.7,0.5) };
    glm::vec2 uv[4] = { glm::vec2(-0.2,-0.2),
                        glm::vec2(1.2,-0.2),
                        glm::vec2(1.2,1.2),
                        glm::vec2(-0.2,1.2) };

    GLuint idx[6] = { 0, 1, 2, 0, 2, 3 };

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &idx[0], GL_STATIC_DRAW);

    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), &pos[0][0], GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), &col[0][0], GL_STATIC_DRAW);

    generateUV();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);
    context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), &uv[0][0], GL_STATIC_DRAW);

}
