#ifndef PLAYER_H
#define PLAYER_H

#include "drawable.h"
#include "scene/terrain.h"
#include "camera.h"

class Terrain;

class Player : public Drawable
{
private:
    bool mKeyPressedW;
    bool mKeyPressedA;
    bool mKeyPressedS;
    bool mKeyPressedD;
    bool mKeyPressedQ;
    bool mKeyPressedE;
    bool mMousePressedLMB;
    bool mMousePressedRMB;
    bool mMouseMoved;
    bool mKeyPressedSpace;
    float mSpeed;
    float mAccelVerti;
    float mSpeedVerti;
    bool mOnGround;
    glm::vec3 mUp;
    glm::vec3 mForward;
    std::vector<glm::vec3> mVertex;
    glm::vec2 mMousePos;

    bool collisionTerrain(glm::vec3 newPos);
    int collisionTerrainEX(glm::vec3 pos, glm::vec3 *newPos, bool *onGround);
    void deleteBlock();
    void addBlock();
    void borderCheck();
    int rayCube(glm::vec3 r, glm::vec3 origin, glm::vec3 dir, glm::vec3 *minP, float *minT);
    bool rayPlane(glm::vec3 origin, glm::vec3 dir, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 *p, float *t);
public:
    Terrain* mpTerrain;
    Camera* mpCamera;
    glm::vec3 mPos;
    bool mAllowGravity;
    Player(OpenGLContext* context, glm::vec3 pos = glm::vec3(0.f,0,0));
    ~Player();
    void create();
    void createQuad(int vid[4], glm::vec4 nor, glm::vec4 col,
                    std::vector<glm::vec4> &posV,
                    std::vector<glm::vec4> &norV,
                    std::vector<glm::vec4> &colV,
                    std::vector<GLuint> &idxV);
    void myKeyPressEvent(QKeyEvent *e);
    void myKeyReleaseEvent(QKeyEvent *e);
    void myMouseMoveEvent(QMouseEvent *e, int width, int height);
    void myMousePressEvent(QMouseEvent *e);
    void myMouseReleaseEvent(QMouseEvent *e);
    void myUpdate(float deltaTime);
};

#endif // PLAYER_H
