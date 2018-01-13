#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QDateTime>

#define SHADOW_RESOLUTION 4096

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      mp_geomCube(new Cube(this)),
      mp_worldAxes(new WorldAxes(this)),
      mp_progLambert(new ShaderProgram(this)), mp_progFlat(new ShaderProgram(this)),
      mp_progMyLambert(new ShaderProgram(this)), mp_progLambertWater(new ShaderProgram(this)),
      mp_progScreen(new ShaderProgram(this)), mp_progPost(new ShaderProgram(this)),
      mp_camera(new Camera()), mp_progShadow(new ShaderProgram(this)),
      mp_terrain(new Terrain(this,0)), mp_terrain_water(new Terrain(this,1)),
      debugCamera(false), drawPlayer(false), drawShadow(false), mp_progCube(new ShaderProgram(this)),
      mp_player(new Player(this, glm::vec3(3,MaxY+2,3))), mp_screen(new Screen(this)),
      mp_texture(new Texture(this)), mp_lSystem(new LSystem()), mp_textureCube(new Texture(this))
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    setFocusPolicy(Qt::ClickFocus);

    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::BlankCursor); // Make the cursor invisible
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    mp_geomCube->destroy();

    delete mp_geomCube;
    delete mp_worldAxes;
    delete mp_progLambert;
    delete mp_progFlat;
    delete mp_progMyLambert;
    delete mp_progLambertWater;
    delete mp_camera;
    delete mp_terrain;
    delete mp_terrain_water;
    delete mp_player;
    delete mp_texture;
    delete mp_lSystem;
    delete mp_screen;
    delete mp_progScreen;
    delete mp_progPost;
    delete mp_progShadow;
    delete mp_textureCube;
    delete mp_progCube;
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
    glCullFace(GL_BACK);
    //glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of Cube
    mp_geomCube->create();
    mp_worldAxes->create();

    // Create and set up the diffuse shader
    mp_progLambert->create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    mp_progFlat->create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    mp_progMyLambert->create(":/glsl/mylambert.vert.glsl", ":/glsl/mylambert.frag.glsl");
    mp_progLambertWater->create(":/glsl/lambertwater.vert.glsl", ":/glsl/lambertwater.frag.glsl");
    mp_progScreen->create(":/glsl/screen.vert.glsl", ":/glsl/screen.frag.glsl");
    mp_progPost->create(":/glsl/post.vert.glsl", ":/glsl/post.frag.glsl");
    mp_progShadow->create(":/glsl/shadow.vert.glsl", ":/glsl/shadow.frag.glsl");
    mp_progCube->create(":/glsl/cube.vert.glsl", ":/glsl/cube.frag.glsl");
    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    mp_progLambert->setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    //    vao.bind();
    glBindVertexArray(vao);

    //mp_terrain->createTestChunk();
    mp_player->mpTerrain = mp_terrain;
    mp_player->mpTerrainWater = mp_terrain_water;
    mp_terrain_water->mExtraTerrain = mp_terrain;
    mp_lSystem->generate(100);
    //mp_lSystem->printAll();
    mp_terrain_water->mLSystem = mp_lSystem;
    mp_terrain->mExtraTerrain = mp_terrain_water;
    mp_terrain->createChunkEX();
    mp_terrain_water->createChunkWater();
    mp_terrain->mergeTerrain();
    mp_player->create();
    mp_screen->create();
    lastTime = QDateTime::currentMSecsSinceEpoch();
    curTime = QDateTime::currentMSecsSinceEpoch();


    mp_curCamera = mp_player->mpCamera;//initial camera
    mp_texture->create(":/texture/all.png");
    mp_texture->load(2);
    mp_texture->bind(2);

    mp_textureCube->createCube(":/texture/front.png",":/texture/left.png",":/texture/right.png",
                               ":/texture/back.png",":/texture/up.png",":/texture/down.png");
    mp_textureCube->loadCube(3);
    mp_textureCube->bindCube(3);

    mp_progLambert->setTexture(2);
    mp_progLambert->setShadow(0);
    mp_progLambertWater->setTexture(2);
    mp_progPost->setTexture(0);
    mp_progCube->setCube(3);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    createRenderBuffers();
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
    mp_progMyLambert->setViewProjMatrix(viewproj);
    mp_progLambertWater->setViewProjMatrix(viewproj);

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

void MyGL::createRenderBuffers()
{
    glActiveTexture(GL_TEXTURE0);

    // Initialize the frame buffers and render textures
    glGenFramebuffers(1, &m_frameBuffer);
    glGenTextures(1, &m_renderedTexture);
    glGenRenderbuffers(1, &m_depthRenderBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    // Bind our texture so that all functions that deal with textures will interact with this one
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    // Give an empty image to OpenGL ( the last "0" )
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0, GL_RGB, GL_FLOAT, (void*)0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, (void*)0);


    // Set the render settings for the texture we've just created.
    // Essentially zero filtering on the "texture" so it appears exactly as rendered
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Clamp the colors at the edge of our texture
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glm::vec4 col(1,1,1,1);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &col[0]);

    // Initialize our depth buffer
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

    // Set m_renderedTexture as the color output of our frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);

    // Sets the color output of the fragment shader to be stored in GL_COLOR_ATTACHMENT0, which we previously set to m_renderedTextures[i]
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers); // "1" is the size of drawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Frame buffer did not initialize correctly..." << std::endl;
        printGLErrorLog();
    }
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL()
{
    //logic update//
    curTime = QDateTime::currentMSecsSinceEpoch();
    float deltaTime = (curTime - lastTime)/1000.f;
    lastTime = curTime;

    mp_player->myUpdate(deltaTime);

    mp_progLambert->setTime(curTime/100);
    mp_progLambertWater->setTime(curTime/100);
    //logic update//

    //setup buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glViewport(0,0,SHADOW_RESOLUTION,SHADOW_RESOLUTION);
    //glViewport(0,0,this->width(),this->height());
    //setup buffer

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    glm::mat4 lightVP = glm::ortho(-60.f,60.f,-60.f,60.f,-30.f,30.f)*glm::lookAt(mp_player->mPos + glm::vec3(-5,5,-5),mp_player->mPos + glm::vec3(5,-5,5),glm::vec3(0,1,0));
    if(true)//mp_curCamera!=nullptr)
    {
        mp_progShadow->setViewProjMatrix(lightVP);//land
//        mp_progShadow->setViewProjMatrix(mp_curCamera->getViewProj());//player
//        mp_progShadow->setViewProjMatrix(mp_curCamera->getViewProj());//water
    }

    //Draw Da Shadow//
    GLDrawShadow(mp_progShadow);
    //Draw Da Shadow//

    ///////////////////////////second pass////////////////////////////////////////
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV//
    ///////////////////////////second pass////////////////////////////////////////

    if(!drawShadow)
    {
    //setup screen
    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    glViewport(0,0,this->width(),this->height());
    //setup screen

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setup shadow texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    mp_progLambert->setShadowMatrix(lightVP);
    //setup shadow texture

    if(mp_curCamera!=nullptr)
    {
        mp_progFlat->setViewProjMatrix(mp_curCamera->getViewProj());//axis
        mp_progLambert->setViewProjMatrix(mp_curCamera->getViewProj());//land
        mp_progMyLambert->setViewProjMatrix(mp_curCamera->getViewProj());//player
        mp_progLambertWater->setViewProjMatrix(mp_curCamera->getViewProj());//water
        mp_progLambertWater->setWorldEye(mp_curCamera->eye);//water
        mp_progCube->setViewProjMatrix(mp_curCamera->getViewProj());//skybox
//        mp_progFlat->setViewProjMatrix(lightVP);//axis
//        mp_progLambert->setViewProjMatrix(lightVP);//land
//        mp_progMyLambert->setViewProjMatrix(lightVP);//player
//        mp_progLambertWater->setViewProjMatrix(lightVP);//water
//        mp_progLambertWater->setWorldEye(mp_curCamera->eye);//water
    }


    //Draw Da Scene//
    GLDrawScene();
    //Draw Da Scene//

//    //draw light
//    glDisable(GL_DEPTH_TEST);
//    mp_progFlat->setModelMatrix(glm::translate(glm::mat4(),mp_player->mPos + glm::vec3(-MaxX/2,MaxY/4,-MaxZ/2)));
//    mp_progFlat->draw(*mp_worldAxes);
//    glEnable(GL_DEPTH_TEST);
}
    else
    {
    performPostprocessRenderPass();
    }
}

void MyGL::performPostprocessRenderPass()
{
    mp_progPost->setTime(QDateTime::currentMSecsSinceEpoch());

    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    glViewport(0,0,this->width(),this->height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);

    mp_progPost->draw(*mp_screen);
}

void MyGL::GLDrawShadow(ShaderProgram* shadowProg)
{
    glDisable(GL_BLEND);

//    if(drawPlayer)
//    {
//        //printf("draw player ");
//        shadowProg->setModelMatrix(glm::translate(glm::mat4(),mp_player->mPos));
//        shadowProg->draw(*mp_player);
//    }

    int chunkCount = mp_terrain->mChunkVector.size();
    mp_texture->bind(2);
    for(int i = 0;i<chunkCount;i++)
    {
        Chunk* chunk = mp_terrain->mChunkVector.at(i);
        shadowProg->setModelMatrix(glm::translate(glm::mat4(),glm::vec3(chunk->mPos.x,0,chunk->mPos.y)));
        shadowProg->draw(*chunk);
        //printf("%d,%d,%d,%d\n",chunk->mPos.x,chunk->mPos.y,chunk->bindPos(),chunk->bufPos);
        //mp_progLambert->drawEX(*chunk);//
    }

//    glDepthMask(GL_FALSE);
//    int chunkCountWater = mp_terrain_water->mChunkVector.size();
//    mp_texture->bind(2);
//    for(int i = 0;i<chunkCountWater;i++)
//    {
//        Chunk* chunk = mp_terrain_water->mChunkVector.at(i);
//        shadowProg->setModelMatrix(glm::translate(glm::mat4(),glm::vec3(chunk->mPos.x,0,chunk->mPos.y)));
//        shadowProg->draw(*chunk);
//        //mp_progLambert->drawEX(*chunk);//
//    }
//    glDepthMask(GL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MyGL::GLDrawScene()
{
    glDepthMask(GL_FALSE);
    mp_textureCube->bindCube(3);
    mp_progCube->setModelMatrix
            (glm::translate(glm::mat4(),mp_player->mpCamera->eye)
             *glm::scale(glm::mat4(),glm::vec3(1000,1000,1000)));
    mp_progCube->draw(*mp_geomCube);
    glDepthMask(GL_TRUE);

    if(drawPlayer)
    {
        //printf("draw player ");
        mp_progMyLambert->setModelMatrix(glm::translate(glm::mat4(),mp_player->mPos));
        mp_progMyLambert->draw(*mp_player);
    }

    int chunkCount = mp_terrain->mChunkVector.size();
    mp_texture->bind(2);
    for(int i = 0;i<chunkCount;i++)
    {
        Chunk* chunk = mp_terrain->mChunkVector.at(i);
        mp_progLambert->setModelMatrix(glm::translate(glm::mat4(),glm::vec3(chunk->mPos.x,0,chunk->mPos.y)));
        mp_progLambert->draw(*chunk);
        //printf("%d,%d,%d,%d\n",chunk->mPos.x,chunk->mPos.y,chunk->bindPos(),chunk->bufPos);
        //mp_progLambert->drawEX(*chunk);//
    }

    glDepthMask(GL_FALSE);
    int chunkCountWater = mp_terrain_water->mChunkVector.size();
    mp_texture->bind(2);
    for(int i = 0;i<chunkCountWater;i++)
    {
        Chunk* chunk = mp_terrain_water->mChunkVector.at(i);
        mp_progLambertWater->setModelMatrix(glm::translate(glm::mat4(),glm::vec3(chunk->mPos.x,0,chunk->mPos.y)));
        mp_progLambertWater->draw(*chunk);
        //mp_progLambert->drawEX(*chunk);//
    }
    glDepthMask(GL_TRUE);


    if(mp_player->mEyeInWater)
    {
        mp_progScreen->draw(*mp_screen);
    }

    glDisable(GL_DEPTH_TEST);
    mp_progFlat->setModelMatrix(glm::mat4());
    mp_progFlat->draw(*mp_worldAxes);
    glEnable(GL_DEPTH_TEST);
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
    else if (e->key() == Qt::Key_H)
    {
        drawShadow = !drawShadow;
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
