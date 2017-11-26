#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QDateTime>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      //mp_geomCube(new Cube(this)),
      mp_worldAxes(new WorldAxes(this)),
      mp_progLambert(new ShaderProgram(this)), mp_progFlat(new ShaderProgram(this)),
      mp_camera(new Camera()),
      mp_terrain(new Terrain(this)), debugCamera(false), drawPlayer(false),
      mp_player(new Player(this, glm::vec3(3,MaxY+2,3)))
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    setFocusPolicy(Qt::ClickFocus);

    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::BlankCursor); // Make the cursor invisible

    mp_curCamera = mp_player->mpCamera;//!!
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    //mp_geomCube->destroy();

    //delete mp_geomCube;
    delete mp_worldAxes;
    delete mp_progLambert;
    delete mp_progFlat;
    delete mp_camera;
    delete mp_terrain;
    delete mp_player;
}


void MyGL::MoveMouseToCenter()
{
    QCursor::setPos(this->mapToGlobal(QPoint(width() / 2, height() / 2)));
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of Cube
    //mp_geomCube->create();
    mp_worldAxes->create();

    // Create and set up the diffuse shader
    mp_progLambert->create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    mp_progFlat->create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    mp_progLambert->setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    //    vao.bind();
    glBindVertexArray(vao);

    //mp_terrain->createTestChunk();
    mp_terrain->createChunkEX();
    mp_player->create();
    mp_player->mpTerrain = mp_terrain;
    lastTime = QDateTime::currentMSecsSinceEpoch();
    curTime = QDateTime::currentMSecsSinceEpoch();
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    mp_curCamera->reset(w,h);
    glm::mat4 viewproj = mp_curCamera->getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    mp_progLambert->setViewProjMatrix(viewproj);
    mp_progFlat->setViewProjMatrix(viewproj);

    printGLErrorLog();
}


// MyGL's constructor links timerUpdate() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to use timerUpdate
void MyGL::timerUpdate()
{
    if(!debugCamera)
        MoveMouseToCenter();
    update();
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mp_curCamera!=nullptr)
    {
        mp_progFlat->setViewProjMatrix(mp_curCamera->getViewProj());
        mp_progLambert->setViewProjMatrix(mp_curCamera->getViewProj());
    }

    GLDrawScene();

    glDisable(GL_DEPTH_TEST);
    mp_progFlat->setModelMatrix(glm::mat4());
    mp_progFlat->draw(*mp_worldAxes);
    glEnable(GL_DEPTH_TEST);
}

void MyGL::GLDrawScene()
{
    curTime = QDateTime::currentMSecsSinceEpoch();
    float deltaTime = (curTime - lastTime)/1000.f;
    lastTime = curTime;

    mp_player->myUpdate(deltaTime);

    if(drawPlayer)
    {
        mp_progLambert->setModelMatrix(glm::translate(glm::mat4(),mp_player->mPos));
        mp_progLambert->draw(*mp_player);
    }

    int chunkCount = mp_terrain->mChunkVector.size();
    for(int i = 0;i<chunkCount;i++)
    {
        Chunk* chunk = mp_terrain->mChunkVector.at(i);
        mp_progLambert->setModelMatrix(glm::translate(glm::mat4(),glm::vec3(chunk->mPos.x,0,chunk->mPos.y)));

        mp_progLambert->drawEX(*chunk);
    }

}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
    else if (e->key() == Qt::Key_R)
    {
        mp_player->mPos = glm::vec3(3,MaxY+2,3);
    }
    else if (e->key() == Qt::Key_T)
    {
        printf("%f,%f,%f\n", mp_player->mPos.x,mp_player->mPos.y,mp_player->mPos.z);
    }
    else if (e->key() == Qt::Key_F)
    {
        mp_camera->reset(this->width(),this->height(),
                         mp_player->mPos-glm::vec3(0,0,10), mp_player->mPos);
        mp_camera->RecomputeAttributes();
    }
    else if (e->key() == Qt::Key_C)
    {
        debugCamera = !debugCamera;
        drawPlayer = !drawPlayer;
        if(!debugCamera)
            setCursor(Qt::BlankCursor);
        else
            setCursor(Qt::ArrowCursor);
        mp_curCamera = debugCamera ? mp_camera : mp_player->mpCamera;
        mp_curCamera->RecomputeAttributes();
    }
    else if (e->key() == Qt::Key_G)
    {
        mp_player->mAllowGravity = !mp_player->mAllowGravity;
    }

    mp_player->myKeyPressEvent(e);
}

void MyGL::keyReleaseEvent(QKeyEvent *e)
{
    mp_player->myKeyReleaseEvent(e);
}

void MyGL::mousePressEvent(QMouseEvent *e)
{
    if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier) == true)
    {
        glm::vec2 p(e->pos().x(), this->height() - e->pos().y());
        glm::vec4 pos((p.x/(float)mp_camera->width)*2.f-1.f, (p.y/(float)mp_camera->height)*2.f-1.f, -1, 1);
        pos = glm::inverse(mp_camera->getViewProj()) * pos;
        pos = pos/pos.w;

        if(e->buttons() & (Qt::LeftButton | Qt::RightButton))
        {
            cameraPrevPos = glm::vec2(e->pos().x(), e->pos().y());
        }
    }

    if(!debugCamera)
    {
        mp_player->myMousePressEvent(e);
    }
}

void MyGL::mouseReleaseEvent(QMouseEvent *e)
{
    if(!debugCamera)
    {
        mp_player->myMouseReleaseEvent(e);
    }
}

void MyGL::mouseMoveEvent(QMouseEvent *e)
{
    if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier) == true)
    {
        glm::vec2 pos(e->pos().x(), e->pos().y());
        if(e->buttons() & Qt::RightButton)
        {
            // Rotation
            glm::vec2 diff = 0.01f * (pos - cameraPrevPos);
            cameraPrevPos = pos;
            mp_camera->RotatePhi(-diff.x);
            mp_camera->RotateTheta(-diff.y);
        }
        else if(e->buttons() & Qt::LeftButton)
        {
            // Panning
            glm::vec2 diff = 0.01f * (pos - cameraPrevPos);
            cameraPrevPos = pos;
            mp_camera->TranslateAlongRight(-diff.x);
            mp_camera->TranslateAlongUp(diff.y);
        }
    }

    if(!debugCamera)
    {
        mp_player->myMouseMoveEvent(e, width(), height());
    }
}

void MyGL::wheelEvent(QWheelEvent *e)
{
    if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier) == true)
    {
        mp_camera->Zoom(e->delta() * 0.005f);
        mp_camera->RecomputeAttributes();
    }
}
