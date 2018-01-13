#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cube.h>
#include <scene/worldaxes.h>
#include "camera.h"
#include <scene/terrain.h>
#include "player.h"
#include "texture.h"
#include "scene/lsystem.h"
#include "scene/screen.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class MyGL : public OpenGLContext
{
    Q_OBJECT
private:
    //Cube* mp_geomCube;// The instance of a unit cube we can use to render any cube. Should NOT be used in final version of your project.
    WorldAxes* mp_worldAxes; // A wireframe representation of the world axes. It is hard-coded to sit centered at (32, 128, 32).
    ShaderProgram* mp_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram* mp_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram* mp_progMyLambert;
    ShaderProgram* mp_progLambertWater;
    ShaderProgram* mp_progScreen;

    ShaderProgram* mp_progPost;//
    ShaderProgram* mp_progShadow;//

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera* mp_camera;
    Camera* mp_curCamera;
    Terrain* mp_terrain;
    Terrain* mp_terrain_water;
    Player* mp_player;
    Screen* mp_screen;

    bool debugCamera;
    bool drawPlayer;

    bool drawShadow;

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;

    void MoveMouseToCenter(); // Forces the mouse position to the screen's center. You should call this
                              // from within a mouse move event after reading the mouse movement so that
                              // your mouse stays within the screen bounds and is always read.

    qint64 lastTime;
    qint64 curTime;
    glm::vec2 cameraPrevPos;

    Texture* mp_texture;

    Texture* mp_textureCube;//
    Cube* mp_geomCube;//
    ShaderProgram* mp_progCube;//

    LSystem* mp_lSystem;

    void createRenderBuffers();
    void performPostprocessRenderPass();
    GLuint m_frameBuffer;
    GLuint m_renderedTexture;
    GLuint m_depthRenderBuffer;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void GLDrawScene();
    void GLDrawShadow(ShaderProgram* shadowProg);

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();
};


#endif // MYGL_H
