#include "player.h"
#include <QKeyEvent>
#include <utility>

Player::Player(OpenGLContext* context, glm::vec3 pos) :
    Drawable(context), mPos(pos), mForward(1,0,0), mUp(0,1,0), mSpeed(3),
    mAccelVerti(-4), mSpeedVerti(0), mOnGround(false), mAllowGravity(true),
    mKeyPressedW(false), mKeyPressedA(false), mKeyPressedS(false), mKeyPressedD(false),
    mKeyPressedQ(false), mKeyPressedE(false), mKeyPressedSpace(false),
    mMousePressedLMB(false), mMousePressedRMB(false),
    mMousePos(0,0), mMouseMoved(false),
    mpTerrain(nullptr), mpCamera(new Camera())
{
    mpCamera->ref = this->mPos + mpCamera->ref - mpCamera->eye + glm::vec3(0.5,1.8,0.5);
    mpCamera->eye = this->mPos + glm::vec3(0.5,1.8,0.5);


    mVertex.push_back(glm::vec3(0,0,0));
    mVertex.push_back(glm::vec3(1,0,0));
    mVertex.push_back(glm::vec3(1,0,1));
    mVertex.push_back(glm::vec3(0,0,1));
    mVertex.push_back(glm::vec3(0,2,0));
    mVertex.push_back(glm::vec3(1,2,0));
    mVertex.push_back(glm::vec3(1,2,1));
    mVertex.push_back(glm::vec3(0,2,1));
}

Player::~Player()
{
    delete mpCamera;
    //no need to delete mpTerrain b/c it's not created by player
    destroy();
}

int Player::rayCube(glm::vec3 r, glm::vec3 origin, glm::vec3 dir, glm::vec3 *minP, float *minT)
{
    //x=0 plane
    if(dir.x>0)
    {
        glm::vec3 p(0,0,0);
        float t = 0;

        bool result = rayPlane(origin,dir,
                               glm::vec3(0,0,0),
                               glm::vec3(0,0,r.z),
                               glm::vec3(0,r.y,r.z),
                               glm::vec3(0,r.y,0),
                               &p,&t);
        if(result&&p.y>=0&&p.y<=r.y&&p.z>=0&&p.z<=r.z)
        {
            *minT = t;
            *minP = p;
            return 1;
        }
    }
    //x=r.x plane
    else if(dir.x<0)
    {
        glm::vec3 p(0,0,0);
        float t = 0;

        bool result = rayPlane(origin,dir,
                               glm::vec3(r.x,0,0),
                               glm::vec3(r.x,r.y,0),
                               glm::vec3(r.x,r.y,r.z),
                               glm::vec3(r.x,0,r.z),
                               &p,&t);
        if(result&&p.y>=0&&p.y<=r.y&&p.z>=0&&p.z<=r.z)
        {
            *minT = t;
            *minP = p;
            return 2;
        }
    }

    //y=0 plane
    if(dir.y>0)
    {
        glm::vec3 p(0,0,0);
        float t = 0;

        bool result = rayPlane(origin,dir,
                               glm::vec3(0,0,0),
                               glm::vec3(r.x,0,0),
                               glm::vec3(r.x,0,r.z),
                               glm::vec3(0,0,r.z),
                               &p,&t);
        if(result&&p.x>=0&&p.x<=r.x&&p.z>=0&&p.z<=r.z)
        {
            *minT = t;
            *minP = p;
            return 3;
        }
    }
    //y=r.y plane
    else if(dir.y<0)
    {
        glm::vec3 p(0,0,0);
        float t = 0;

        bool result = rayPlane(origin,dir,
                               glm::vec3(0,r.y,0),
                               glm::vec3(r.x,r.y,0),
                               glm::vec3(r.x,r.y,r.z),
                               glm::vec3(0,r.y,r.z),
                               &p,&t);
        if(result&&p.x>=0&&p.x<=r.x&&p.z>=0&&p.z<=r.z)
        {
            *minT = t;
            *minP = p;
            return 4;
        }
    }

    //z=0 plane
    if(dir.z>0)
    {
        glm::vec3 p(0,0,0);
        float t = 0;

        bool result = rayPlane(origin,dir,
                               glm::vec3(0,0,0),
                               glm::vec3(0,r.y,0),
                               glm::vec3(r.x,r.y,0),
                               glm::vec3(r.x,0,0),
                               &p,&t);
        if(result&&p.x>=0&&p.x<=r.x&&p.y>=0&&p.y<=r.y)
        {
            *minT = t;
            *minP = p;
            return 5;
        }
    }
    //z=r.z plane
    else if(dir.z<0)
    {
        glm::vec3 p(0,0,0);
        float t = 0;

        bool result = rayPlane(origin,dir,
                               glm::vec3(0,0,r.z),
                               glm::vec3(r.x,0,r.z),
                               glm::vec3(r.x,r.y,r.z),
                               glm::vec3(0,r.y,r.z),
                               &p,&t);
        if(result&&p.x>=0&&p.x<=r.x&&p.y>=0&&p.y<=r.y)
        {
            *minT = t;
            *minP = p;
            return 6;
        }
    }

    return 0;
}

bool Player::rayPlane(glm::vec3 origin, glm::vec3 dir, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3* p, float* t)
{
    glm::vec3 n = glm::normalize(glm::cross(b-a,c-a));
    glm::vec4 L = glm::vec4(n,-glm::dot(n,a));
    glm::vec4 S = glm::vec4(origin,1);
    glm::vec4 V = glm::vec4(dir,0);
    float LV = glm::dot(L,V);
    if(LV == 0)
    {
        return false;
    }
    float tempT = - glm::dot(L,S) / LV;
    if(tempT<0)
        return false;
    else
    {
        *t = - glm::dot(L,S) / LV;
        *p = origin + (*t)*dir;
        return true;
    }
}

void Player::createQuad(int vid[4], glm::vec4 nor, glm::vec4 col,
std::vector<glm::vec4> &posV,
std::vector<glm::vec4> &norV,
std::vector<glm::vec4> &colV,
std::vector<GLuint> &idxV)
{
    int idxStart = posV.size();

    for(int i = 0;i<4;i++)
    {
        posV.push_back(glm::vec4(mVertex.at(vid[i]),1));
        norV.push_back(nor);
        colV.push_back(col);

        if(i>2)
        {
            idxV.push_back(idxStart);
            idxV.push_back(idxStart+i-1);
        }

        idxV.push_back(idxStart+i);
    }
}

void Player::create()
{
    std::vector<glm::vec4> tempPosV;
    std::vector<glm::vec4> tempNorV;
    std::vector<glm::vec4> tempColV;

    std::vector<GLuint> tempIdxV;

    int vid[6][4] = { {0,1,2,3},
                      {4,7,6,5},
                      {0,4,5,1},
                      {1,5,6,2},
                      {2,6,7,3},
                      {0,3,7,4} };
    glm::vec4 vnor[6] = { glm::vec4(0,-1,0,0),
                          glm::vec4(0,1,0,0),
                          glm::vec4(0,0,-1,0),
                          glm::vec4(1,0,0,0),
                          glm::vec4(0,0,1,0),
                          glm::vec4(-1,0,0,0) };
    glm::vec4 vcol[6] = { glm::vec4(1,0,0,1),
                          glm::vec4(0,1,0,1),
                          glm::vec4(0,0,1,1),
                          glm::vec4(1,1,0,1),
                          glm::vec4(1,0,1,1),
                          glm::vec4(0,1,1,1) };

    for(int i = 0;i<6;i++)
    {
        createQuad(vid[i],vnor[i],vcol[i],tempPosV,tempNorV,tempColV,tempIdxV);
    }

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempIdxV.size() * sizeof(GLuint), tempIdxV.data(), GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, tempPosV.size() * sizeof(glm::vec4), tempPosV.data(), GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, tempNorV.size() * sizeof(glm::vec4), tempNorV.data(), GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, tempColV.size() * sizeof(glm::vec4), tempColV.data(), GL_STATIC_DRAW);
}

void Player::myKeyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_W:
        mKeyPressedW = true;
        break;
    case Qt::Key_A:
        mKeyPressedA = true;
        break;
    case Qt::Key_S:
        mKeyPressedS = true;
        break;
    case Qt::Key_D:
        mKeyPressedD = true;
        break;
    case Qt::Key_Q:
        mKeyPressedQ = true;
        break;
    case Qt::Key_E:
        mKeyPressedE = true;
        break;
    case Qt::Key_Space:
        mKeyPressedSpace = true;
    default:
        break;
    }
}

void Player::myKeyReleaseEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_W:
        mKeyPressedW = false;
        break;
    case Qt::Key_A:
        mKeyPressedA = false;
        break;
    case Qt::Key_S:
        mKeyPressedS = false;
        break;
    case Qt::Key_D:
        mKeyPressedD = false;
        break;
    case Qt::Key_Q:
        mKeyPressedQ = false;
        break;
    case Qt::Key_E:
        mKeyPressedE = false;
        break;
    case Qt::Key_Space:
        mKeyPressedSpace = false;
    default:
        break;
    }
}

void Player::myMouseMoveEvent(QMouseEvent *e, int width, int height)
{
    mMouseMoved = true;
    mMousePos = glm::vec2(e->pos().x() - width/2.f,e->pos().y() - height/2.f);
}

void Player::myMousePressEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton)
    {
        mMousePressedLMB = true;
    }

    if(e->buttons()&Qt::RightButton)
    {
        mMousePressedRMB = true;
    }
}

void Player::myMouseReleaseEvent(QMouseEvent *e)
{
    if((!e->buttons())&Qt::LeftButton)
    {
        mMousePressedLMB = false;
    }

    if((!e->buttons())&Qt::RightButton)
    {
        mMousePressedRMB = false;
    }
}

void Player::myUpdate(float deltaTime)
{
//    if(mKeyPressedW||mKeyPressedA||mKeyPressedS||mKeyPressedD||
//            mKeyPressedQ||mKeyPressedE||mKeyPressedSpace||
//            mMousePressedLMB||mMousePressedRMB||mMouseMoved)

        glm::vec3 deltaPos(0,0,0);

        //TO DO
        //gravity
        if(mAllowGravity&&mOnGround)
        {
            mSpeedVerti = 0;
            mAccelVerti = 0;
        }

        if(mKeyPressedW)
        {
            deltaPos += mForward*mSpeed*deltaTime;
        }

        if(mKeyPressedS)
        {
            deltaPos -= mForward*mSpeed*deltaTime;
        }

        if(mKeyPressedA)
        {
            deltaPos += glm::cross(mUp,mForward)*mSpeed*deltaTime;
        }

        if(mKeyPressedD)
        {
            deltaPos -= glm::cross(mUp,mForward)*mSpeed*deltaTime;
        }

        if(mKeyPressedQ)
        {
            deltaPos += mUp*mSpeed*deltaTime;
        }

        if(mKeyPressedE)
        {
            deltaPos -= mUp*mSpeed*deltaTime;
        }

        if(mKeyPressedSpace)
        {
            //TO DO
            //printf("space\n");
            if(mAllowGravity&&mOnGround)
            {
                //printf("jump\n");
                mSpeedVerti = 5;
                mOnGround = false;
            }
            //mKeyPressedSpace = false;
        }

        if(mMouseMoved)
        {
            glm::vec2 diff = 0.05f * (mMousePos);
            mpCamera->RotateAboutUp(-diff.x);
            mpCamera->RotateAboutRight(-diff.y);
            mForward = glm::normalize(glm::vec3(mpCamera->ref.x - mpCamera->eye.x,
                                                0,
                                                mpCamera->ref.z - mpCamera->eye.z));
            mMouseMoved = false;
        }

        //delete block
        if(mMousePressedLMB)
        {
            deleteBlock();
            mMousePressedLMB = false;
        }

        //add block
        if(mMousePressedRMB)
        {
            addBlock();
            mMousePressedRMB = false;
        }

        //x-z plane uniform speed, y axis apply gravity

        if(mAllowGravity&&!mOnGround)
        {
            mAccelVerti = -10;
            mSpeedVerti += deltaTime*mAccelVerti;
            deltaPos += deltaTime*mSpeedVerti*mUp;
        }

        glm::vec3 newPos = mPos + deltaPos;

        mOnGround = false;
        collisionTerrainEX(mPos, &newPos, &mOnGround);//will only return the first collision result even thought we are recursively invoking it

        mPos = newPos;

        borderCheck();

        mpCamera->ref = this->mPos + mpCamera->ref - mpCamera->eye + glm::vec3(0.5,1.8,0.5);
        mpCamera->eye = mPos + glm::vec3(0.5,1.8,0.5);

}

void Player::borderCheck()
{

    glm::vec3 offset[4] = {
        glm::vec3(2,0,0),
        glm::vec3(-2,0,0),
        glm::vec3(0,0,2),
        glm::vec3(0,0,-2)
    };
    for(int i = 0;i<4;i++)
    {
        glm::ivec2 chunkXZ = mpTerrain->toChunkXZ(mPos + offset[i]);
        if(mpTerrain->mChunkMap.count(std::make_pair(chunkXZ.x,chunkXZ.y))==0)
        {
            //mpTerrain->createTestChunk(chunkXZ);
            //mpTerrain->createChunk(chunkXZ);
            mpTerrain->createChunkEX(chunkXZ);
        }
    }
}

void Player::deleteBlock()
{
    glm::vec3 worldDir = glm::normalize(mpCamera->ref - mpCamera->eye);
    glm::vec3 worldOrigin = mpCamera->eye;
    glm::vec3 r(1,1,1);
    float minT = std::numeric_limits<float>::max();
    glm::vec3 minP(0,0,0);
    glm::vec3 minWorldTarget(0,0,0);
    bool result = false;

    for(int i = -2;i<3;i++)
    {
        for(int j = -2;j<3;j++)
        {
            for(int k = -2;k<3;k++)
            {
                if(i==0&&j==0&&k==0)
                    continue;
                else
                {
                    glm::vec3 worldTarget = mpCamera->eye + glm::vec3(i,j,k);
                    //if(mpTerrain->getBlockAtEX(mpCamera->eye + glm::vec3(i,j,k))==EMPTY)
                    if(mpTerrain->getBlockAtEX(worldTarget)==EMPTY)
                    {
                        continue;
                    }
                    glm::vec3 localDir = worldDir;
                    glm::vec3 worldToLocal = glm::floor(worldTarget);
                    glm::vec3 localOrigin = worldOrigin - worldToLocal;
                    glm::vec3 p(0,0,0);
                    float t = 0;

                    if(rayCube(r,localOrigin,localDir,&p,&t)!=0)
                    {
                        if(t<minT)
                        {
                            minT = t;
                            p = p+worldToLocal;
                            minP = p;
                            minWorldTarget = worldTarget;
                        }
                        result |= true;
                    }

                }
            }
        }
    }

    if(result)
    {
        mpTerrain->setBlockAtEX(minWorldTarget,EMPTY);
        glm::ivec2 chunkXZ = mpTerrain->toChunkXZ(minWorldTarget);
        glm::ivec3 blockXYZ = mpTerrain->toBlockXYZ(minWorldTarget);
        if(blockXYZ.x==MaxX-1)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x+MaxX,chunkXZ.y);
            if(chunk!=nullptr) chunk->createEXX();
        }
        if(blockXYZ.x==0)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x-MaxX,chunkXZ.y);
            if(chunk!=nullptr) chunk->createEXX();
        }
        if(blockXYZ.z==MaxZ-1)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x,chunkXZ.y+MaxZ);
            if(chunk!=nullptr) chunk->createEXX();
        }
        if(blockXYZ.z==0)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x,chunkXZ.y-MaxZ);
            if(chunk!=nullptr) chunk->createEXX();
        }
        Chunk* chunk = mpTerrain->getChunkAtEX(minWorldTarget);
        if(chunk!=nullptr) chunk->createEXX();
    }
}

void Player::addBlock()
{
    glm::vec3 worldDir = glm::normalize(mpCamera->ref - mpCamera->eye);
    glm::vec3 worldOrigin = mpCamera->eye;
    glm::vec3 r(1,1,1);
    float maxT = std::numeric_limits<float>::min();
    glm::vec3 maxP(0,0,0);
    glm::vec3 maxWorldTarget(0,0,0);
    bool result = false;

    for(int i = -2;i<3;i++)
    {
        for(int j = -2;j<3;j++)
        {
            for(int k = -2;k<3;k++)
            {
                if(i==0&&j==0&&k==0)
                    continue;
                else
                {
                    glm::vec3 worldTarget = mpCamera->eye + glm::vec3(i,j,k);
                    //if(mpTerrain->getBlockAtEX(mpCamera->eye + glm::vec3(i,j,k))==EMPTY)
                    if(mpTerrain->getBlockAtEX(worldTarget)!=EMPTY)
                    {
                        continue;
                    }
                    glm::vec3 localDir = worldDir;
                    glm::vec3 worldToLocal = glm::floor(worldTarget);
                    glm::vec3 localOrigin = worldOrigin - worldToLocal;
                    glm::vec3 p(0,0,0);
                    float t = 0;

                    if(rayCube(r,localOrigin,localDir,&p,&t)!=0)
                    {
                        if(t>maxT)
                        {
                            maxT = t;
                            p = p+worldToLocal;
                            maxP = p;
                            maxWorldTarget = worldTarget;
                        }
                        result |= true;
                    }

                }
            }
        }
    }

    if(result)
    {
        mpTerrain->setBlockAtEX(maxWorldTarget,STONE);
        glm::ivec2 chunkXZ = mpTerrain->toChunkXZ(maxWorldTarget);
        glm::ivec3 blockXYZ = mpTerrain->toBlockXYZ(maxWorldTarget);
        if(blockXYZ.x==MaxX-1)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x+MaxX,chunkXZ.y);
            if(chunk!=nullptr) chunk->createEXX();
        }
        if(blockXYZ.x==0)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x-MaxX,chunkXZ.y);
            if(chunk!=nullptr) chunk->createEXX();
        }
        if(blockXYZ.z==MaxZ-1)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x,chunkXZ.y+MaxZ);
            if(chunk!=nullptr) chunk->createEXX();
        }
        if(blockXYZ.z==0)
        {
            Chunk* chunk = mpTerrain->getChunkAt(chunkXZ.x,chunkXZ.y-MaxZ);
            if(chunk!=nullptr) chunk->createEXX();
        }
        Chunk* chunk = mpTerrain->getChunkAtEX(maxWorldTarget);
        if(chunk!=nullptr) chunk->createEXX();
    }
}

bool Player::collisionTerrain(glm::vec3 newPos)
{
    bool result = false;
    if(mpTerrain!=nullptr)
    {
        glm::vec3 offset[12] = { glm::vec3(0,0,0),
                                 glm::vec3(1,0,0),
                                 glm::vec3(1,0,1),
                                 glm::vec3(0,0,1),
                                 glm::vec3(0,1,0),
                                 glm::vec3(1,1,0),
                                 glm::vec3(1,1,1),
                                 glm::vec3(0,1,1),
                                 glm::vec3(0,2,0),
                                 glm::vec3(1,2,0),
                                 glm::vec3(1,2,1),
                                 glm::vec3(0,2,1) };

        for(int i = 0;i<12;i++)
        {

            glm::vec3 posTemp = newPos + offset[i];

            if(mpTerrain->getBlockAtEX(posTemp)!=EMPTY)
            {
                result = true;
                break;
            }
        }

    }
    return result;
}

int Player::collisionTerrainEX(glm::vec3 pos, glm::vec3 *newPos, bool *onGround)
{
    int result = 0;

    if(mpTerrain!=nullptr)
    {
        glm::vec3 offset[12] = { glm::vec3(0,0,0),
                                 glm::vec3(0.99,0,0),
                                 glm::vec3(0.99,0,0.99),
                                 glm::vec3(0,0,0.99),
                                 glm::vec3(0,0.99,0),
                                 glm::vec3(0.99,0.99,0),
                                 glm::vec3(0.99,0.99,0.99),
                                 glm::vec3(0,0.99,0.99),
                                 glm::vec3(0,1.99,0),
                                 glm::vec3(0.99,1.99,0),
                                 glm::vec3(0.99,1.99,0.99),
                                 glm::vec3(0,1.99,0.99) };

        glm::vec3 worldDir = *newPos - pos;
        glm::vec3 r(1,1,1);
        float minT = std::numeric_limits<float>::max();

        for(int n = 0;n<12;n++)
        {
            glm::vec3 worldOrigin = pos + offset[n];
            glm::vec3 worldDestination = *newPos + offset[n];

            glm::ivec3 posGrid = (glm::ivec3)glm::floor(worldOrigin);
            glm::ivec3 newPosGrid = (glm::ivec3)glm::floor(worldDestination);

            int xGridStart = posGrid.x <= newPosGrid.x ? posGrid.x : newPosGrid.x;
            int yGridStart = posGrid.y <= newPosGrid.y ? posGrid.y : newPosGrid.y;
            int zGridStart = posGrid.z <= newPosGrid.z ? posGrid.z : newPosGrid.z;
            int xGridEnd = posGrid.x <= newPosGrid.x ? newPosGrid.x : posGrid.x;
            int yGridEnd = posGrid.y <= newPosGrid.y ? newPosGrid.y : posGrid.y ;
            int zGridEnd = posGrid.z <= newPosGrid.z ? newPosGrid.z : posGrid.z;

            for(int i = xGridStart;i<=xGridEnd;i++)
            {
                for(int j = yGridStart;j<=yGridEnd;j++)
                {
                    for(int k = zGridStart;k<=zGridEnd;k++)
                    {
                        glm::vec3 worldToLocal = glm::vec3(i,j,k);
                        glm::vec3 localOrigin = worldOrigin - worldToLocal;
                        glm::vec3 localDir = worldDir;
                        glm::vec3 p(0,0,0);
                        float t = 0;

                        if(mpTerrain->getBlockAt(i,j,k)==EMPTY)
                        {
                            continue;
                        }

                        int rayCubeResult = rayCube(r,localOrigin,localDir,&p,&t);
                        if(rayCubeResult!=0)
                        {
                            if(t<minT)
                            {
                                minT = t;
                                p = p+worldToLocal;
                                result = rayCubeResult;
                            }
                        }


                    }
                }
            }
        }

        if(result!=0)
        {
            //this makes the character slide along the collision sufface
            switch(result)
            {
            case 1:
            case 2:
                newPos->x = pos.x + (newPos->x - pos.x)*(minT-0.01f);
                break;
            case 3:
                newPos->y = pos.y + (newPos->y - pos.y)*(minT-0.01f);
                break;
            case 4:
                *onGround = true | *onGround;
                newPos->y = pos.y + (newPos->y - pos.y)*(minT-0.01f);
                break;
            case 5:
            case 6:
                newPos->z = pos.z + (newPos->z - pos.z)*(minT-0.01f);
                break;
            default:
                break;
            }
            collisionTerrainEX(pos,newPos,onGround);
            //*newPos = pos + (*newPos - pos)*(minT-0.01f);
        }


    }

    return result;
}
