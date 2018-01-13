#pragma once

#include <openglcontext.h>
#include <la.h>
#include <memory>

class Texture
{
public:
    Texture(OpenGLContext* context);
    ~Texture();

    void create(const char *texturePath);
    void load(int texSlot);
    void bind(int texSlot);


    void createCube(const char *front, const char *left,const char *right,
                    const char *back,const char *up,const char *down);
    void loadCube(int texSlot);
    void bindCube(int texSlot);
private:
    OpenGLContext* context;
    GLuint m_textureHandle;
    std::shared_ptr<QImage> m_textureImage;
    std::shared_ptr<QImage> m_frontImage;
    std::shared_ptr<QImage> m_leftImage;
    std::shared_ptr<QImage> m_rightImage;
    std::shared_ptr<QImage> m_backImage;
    std::shared_ptr<QImage> m_upImage;
    std::shared_ptr<QImage> m_downImage;
};
