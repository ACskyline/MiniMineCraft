#include "texture.h"
#include <QImage>
#include <openglcontext.h>

Texture::Texture(OpenGLContext *context)
    : context(context), m_textureHandle(-1), m_textureImage(nullptr),
      m_frontImage(nullptr),m_leftImage(nullptr),m_rightImage(nullptr),
      m_backImage(nullptr),m_upImage(nullptr),m_downImage(nullptr)
{}

Texture::~Texture()
{}

void Texture::create(const char *texturePath)
{
    context->printGLErrorLog();

    QImage img(texturePath);
    img.convertToFormat(QImage::Format_ARGB32);
    img = img.mirrored();
    m_textureImage = std::make_shared<QImage>(img);
    context->glGenTextures(1, &m_textureHandle);

    context->printGLErrorLog();
}

void Texture::createCube(const char *front, const char *left, const char *right,
                         const char *back, const char *up, const char *down)
{
    context->printGLErrorLog();

    QImage img1(front);
    img1.convertToFormat(QImage::Format_ARGB32);
    img1 = img1.mirrored(true,false);
    m_frontImage = std::make_shared<QImage>(img1);

    QImage img2(left);
    img2.convertToFormat(QImage::Format_ARGB32);
    img2 = img2.mirrored(true,false);
    m_leftImage = std::make_shared<QImage>(img2);

    QImage img3(right);
    img3.convertToFormat(QImage::Format_ARGB32);
    img3 = img3.mirrored(true,false);
    m_rightImage = std::make_shared<QImage>(img3);

    QImage img4(back);
    img4.convertToFormat(QImage::Format_ARGB32);
    img4 = img4.mirrored(true,false);
    m_backImage = std::make_shared<QImage>(img4);

    QImage img5(up);
    img5.convertToFormat(QImage::Format_ARGB32);
    img5 = img5.mirrored(true,false);
    m_upImage = std::make_shared<QImage>(img5);

    QImage img6(down);
    img6.convertToFormat(QImage::Format_ARGB32);
    img6 = img6.mirrored(true,false);
    m_downImage = std::make_shared<QImage>(img6);

    context->glGenTextures(1, &m_textureHandle);

    context->printGLErrorLog();
}

void Texture::load(int texSlot = 0)
{
    context->printGLErrorLog();

    context->glActiveTexture(GL_TEXTURE0 + texSlot);
    context->glBindTexture(GL_TEXTURE_2D, m_textureHandle);

    // These parameters need to be set for EVERY texture you create
    // They don't always have to be set to the values given here, but they do need
    // to be set
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                          m_textureImage->width(), m_textureImage->height(),
                          0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_textureImage->bits());
    context->printGLErrorLog();
}

void Texture::loadCube(int texSlot)
{
    context->printGLErrorLog();

    context->glActiveTexture(GL_TEXTURE0 + texSlot);
    context->glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureHandle);

    context->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA,
                          m_frontImage->width(), m_frontImage->height(),
                          0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_frontImage->bits());

    context->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA,
                          m_leftImage->width(), m_leftImage->height(),
                          0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_leftImage->bits());

    context->glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA,
                          m_rightImage->width(), m_rightImage->height(),
                          0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_rightImage->bits());

    context->glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA,
                          m_backImage->width(), m_backImage->height(),
                          0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_backImage->bits());

    context->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA,
                          m_upImage->width(), m_upImage->height(),
                          0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_upImage->bits());

    context->glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA,
                          m_downImage->width(), m_downImage->height(),
                          0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_downImage->bits());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    context->printGLErrorLog();
}


void Texture::bind(int texSlot = 0)
{
    context->glActiveTexture(GL_TEXTURE0 + texSlot);
    context->glBindTexture(GL_TEXTURE_2D, m_textureHandle);
}

void Texture::bindCube(int texSlot)
{
    context->glActiveTexture(GL_TEXTURE0 + texSlot);
    context->glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureHandle);
}
