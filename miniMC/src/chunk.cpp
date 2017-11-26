#include "chunk.h"

Chunk::Chunk(OpenGLContext* context, glm::ivec2 pos) :
    Drawable(context), mBlock{EMPTY}, mPos(pos)
{

}

Chunk::~Chunk()
{
    destroy();
}

glm::vec4 Chunk::getBlockTypeColor(BlockType bt)
{
    glm::vec4 col(0,0,0,0);
    switch(bt)
    {
    case GRASS:
        col = glm::vec4(0.3725f,0.6235f,0.2078f,1.0f);
        break;
    case DIRT:
        col = glm::vec4(0.4745f,0.3333f,0.2275f,1.0f);
        break;
    case STONE:
        col = glm::vec4(0.5f,0.5f,0.5f,1.0f);
        break;
    case EMPTY:
    default:
        col = glm::vec4(1,1,1,1);
        break;
    }
    return col;
}

void Chunk::createSquare(int i, int j, int k,
                         std::vector<glm::vec4> &posV,
                         std::vector<glm::vec4> &norV,
                         std::vector<glm::vec4> &colV,
                         std::vector<GLuint> &idxV)
{
    if(mBlock[i*MaxY*MaxZ + j*MaxZ + k] == EMPTY)
    {
        return;
    }

    const glm::vec4 pos(i,j,k,0);

    const glm::vec4 pos0=pos+glm::vec4(0,0,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos1=pos+glm::vec4(1,0,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos2=pos+glm::vec4(1,0,1,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos3=pos+glm::vec4(0,0,1,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos4=pos+glm::vec4(0,1,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos5=pos+glm::vec4(1,1,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos6=pos+glm::vec4(1,1,1,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos7=pos+glm::vec4(0,1,1,1);//-glm::vec4(0.5,0.5,0.5,0);

    const glm::vec4 nor0(0,-1,0,0);
    const glm::vec4 nor1(0,1,0,0);
    const glm::vec4 nor2(0,0,-1,0);
    const glm::vec4 nor3(1,0,0,0);
    const glm::vec4 nor4(0,0,1,0);
    const glm::vec4 nor5(-1,0,0,0);

    glm::vec4 col = getBlockTypeColor(getBlockType(i,j,k));

    //-x direction
    if((i-1>=0&&i-1<MaxX&&mBlock[(i-1)*MaxY*MaxZ + j*MaxZ + k] == EMPTY) || (i-1<0))
    {
        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos3);
        posV.push_back(pos7);
        posV.push_back(pos4);

        for(int a = 0;a<4;++a)
        {
            norV.push_back(nor5);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+x direction
    if((i+1>=0&&i+1<MaxX&&mBlock[(i+1)*MaxY*MaxZ + j*MaxZ + k] == EMPTY) || (i+1>=MaxX))
    {
        int idstart = posV.size();

        posV.push_back(pos1);
        posV.push_back(pos5);
        posV.push_back(pos6);
        posV.push_back(pos2);

        for(int a = 0;a<4;++a)
        {
            norV.push_back(nor3);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //-y direction
    if((j-1>=0&&j-1<MaxY&&mBlock[i*MaxY*MaxZ + (j-1)*MaxZ + k] == EMPTY) || (j-1<0))
    {
        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos1);
        posV.push_back(pos2);
        posV.push_back(pos3);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor0);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+y direction
    if((j+1>=0&&j+1<MaxY&&mBlock[i*MaxY*MaxZ + (j+1)*MaxZ + k] == EMPTY) || (j+1>=MaxY))
    {
        int idstart = posV.size();

        posV.push_back(pos4);
        posV.push_back(pos7);
        posV.push_back(pos6);
        posV.push_back(pos5);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor1);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //-z direction
    if((k-1>=0&&k-1<MaxZ&&mBlock[i*MaxY*MaxZ + j*MaxZ + k-1] == EMPTY )|| (k-1<0))
    {
        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos4);
        posV.push_back(pos5);
        posV.push_back(pos1);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor2);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+z direction
    if((k+1>=0&&k+1<MaxZ&&mBlock[i*MaxY*MaxZ + j*MaxZ + k+1] == EMPTY )||( k+1>=MaxZ))
    {
        int idstart = posV.size();

        posV.push_back(pos2);
        posV.push_back(pos6);
        posV.push_back(pos7);
        posV.push_back(pos3);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor4);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
}

void Chunk::createSquareEX(int i, int j, int k,
                         std::vector<glm::vec4> &posV,
                         std::vector<glm::vec4> &norV,
                         std::vector<glm::vec4> &colV,
                         std::vector<GLuint> &idxV)
{
    if(mpTerrain==nullptr||mpTerrain->getBlockAt(i,j,k) == EMPTY)
    {
        return;
    }

    int blockI = i - mPos.x;
    int blockJ = j;
    int blockK = k - mPos.y;

    const glm::vec4 pos(blockI,blockJ,blockK,0);

    const glm::vec4 pos0=pos+glm::vec4(0,0,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos1=pos+glm::vec4(1,0,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos2=pos+glm::vec4(1,0,1,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos3=pos+glm::vec4(0,0,1,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos4=pos+glm::vec4(0,1,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos5=pos+glm::vec4(1,1,0,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos6=pos+glm::vec4(1,1,1,1);//-glm::vec4(0.5,0.5,0.5,0);
    const glm::vec4 pos7=pos+glm::vec4(0,1,1,1);//-glm::vec4(0.5,0.5,0.5,0);

    const glm::vec4 nor0(0,-1,0,0);
    const glm::vec4 nor1(0,1,0,0);
    const glm::vec4 nor2(0,0,-1,0);
    const glm::vec4 nor3(1,0,0,0);
    const glm::vec4 nor4(0,0,1,0);
    const glm::vec4 nor5(-1,0,0,0);

    glm::vec4 col = getBlockTypeColor(getBlockType(blockI,blockJ,blockK));

//    printf("%d,%d,%d:%d ",i-1,j,k,mpTerrain->getBlockAt(i-1,j,k));
//    printf("%d,%d,%d:%d ",i+1,j,k,mpTerrain->getBlockAt(i+1,j,k));
//    printf("%d,%d,%d:%d ",i,j-1,k,mpTerrain->getBlockAt(i,j-1,k));
//    printf("%d,%d,%d:%d ",i,j+1,k,mpTerrain->getBlockAt(i,j+1,k));
//    printf("%d,%d,%d:%d ",i,j,k-1,mpTerrain->getBlockAt(i,j,k-1));
//    printf("%d,%d,%d:%d ",i,j,k+1,mpTerrain->getBlockAt(i,j,k+1));

    //-x direction
    if(mpTerrain->getBlockAt(i-1,j,k) == EMPTY)
    {
        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos3);
        posV.push_back(pos7);
        posV.push_back(pos4);

        for(int a = 0;a<4;++a)
        {
            norV.push_back(nor5);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+x direction
    if(mpTerrain->getBlockAt(i+1,j,k) == EMPTY)
    {
        int idstart = posV.size();

        posV.push_back(pos1);
        posV.push_back(pos5);
        posV.push_back(pos6);
        posV.push_back(pos2);

        for(int a = 0;a<4;++a)
        {
            norV.push_back(nor3);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //-y direction
    if(mpTerrain->getBlockAt(i,j-1,k) == EMPTY)
    {
        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos1);
        posV.push_back(pos2);
        posV.push_back(pos3);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor0);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+y direction
    if(mpTerrain->getBlockAt(i,j+1,k) == EMPTY)
    {
        int idstart = posV.size();

        posV.push_back(pos4);
        posV.push_back(pos7);
        posV.push_back(pos6);
        posV.push_back(pos5);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor1);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //-z direction
    if(mpTerrain->getBlockAt(i,j,k-1) == EMPTY)
    {
        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos4);
        posV.push_back(pos5);
        posV.push_back(pos1);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor2);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+z direction
    if(mpTerrain->getBlockAt(i,j,k+1) == EMPTY)
    {
        int idstart = posV.size();

        posV.push_back(pos2);
        posV.push_back(pos6);
        posV.push_back(pos7);
        posV.push_back(pos3);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor4);
            colV.push_back(col);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
}


void Chunk::create()
{
    destroy();

    std::vector<glm::vec4> tempPosV;
    std::vector<glm::vec4> tempNorV;
    std::vector<glm::vec4> tempColV;

    std::vector<GLuint> tempIdxV;

    for(int i = 0;i<MaxX;i++)
    {
        for(int j = 0;j<MaxY;j++)
        {
            for(int k = 0;k<MaxZ;k++)
            {
                createSquare(i,j,k,tempPosV,tempNorV,tempColV,tempIdxV);
            }
        }
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

void Chunk::createEX()
{
    destroy();

    std::vector<glm::vec4> tempPosV;
    std::vector<glm::vec4> tempNorV;
    std::vector<glm::vec4> tempColV;

    std::vector<GLuint> tempIdxV;

    for(int i = mPos.x;i<mPos.x + MaxX;i++)
    {
        for(int j = 0;j<MaxY;j++)
        {
            for(int k = mPos.y;k<mPos.y + MaxZ;k++)
            {
                createSquareEX(i,j,k,tempPosV,tempNorV,tempColV,tempIdxV);
            }
        }
    }

    //printf("size:%d,%d,%d,%d ",tempIdxV.size(),tempPosV.size(),tempNorV.size(),tempColV.size());

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

void Chunk::createEXX()
{
    destroy();

    std::vector<glm::vec4> tempPosV;
    std::vector<glm::vec4> tempNorV;
    std::vector<glm::vec4> tempColV;
    std::vector<glm::vec4> tempCombineV;

    std::vector<GLuint> tempIdxV;

    for(int i = mPos.x;i<mPos.x + MaxX;i++)
    {
        for(int j = 0;j<MaxY;j++)
        {
            for(int k = mPos.y;k<mPos.y + MaxZ;k++)
            {
                createSquareEX(i,j,k,tempPosV,tempNorV,tempColV,tempIdxV);
            }
        }
    }

    for(int i = 0;i<tempPosV.size();i++)
    {
        tempCombineV.push_back(tempPosV.at(i));
        tempCombineV.push_back(tempNorV.at(i));
        tempCombineV.push_back(tempColV.at(i));
    }
    //printf("size:%d,%d,%d,%d ",tempIdxV.size(),tempPosV.size(),tempNorV.size(),tempColV.size());

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

    generateCombine();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCombine);
    context->glBufferData(GL_ARRAY_BUFFER, tempCombineV.size() * sizeof(glm::vec4), tempCombineV.data(), GL_STATIC_DRAW);

//    generatePos();
//    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
//    context->glBufferData(GL_ARRAY_BUFFER, tempPosV.size() * sizeof(glm::vec4), tempPosV.data(), GL_STATIC_DRAW);

//    generateNor();
//    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
//    context->glBufferData(GL_ARRAY_BUFFER, tempNorV.size() * sizeof(glm::vec4), tempNorV.data(), GL_STATIC_DRAW);

//    generateCol();
//    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
//    context->glBufferData(GL_ARRAY_BUFFER, tempColV.size() * sizeof(glm::vec4), tempColV.data(), GL_STATIC_DRAW);
}

BlockType Chunk::getBlockType(int x, int y, int z)
{
    if(x>=0&&x<MaxX&&y>=0&&y<MaxY&&z>=0&&z<MaxZ)
        return mBlock[x*MaxY*MaxZ+y*MaxZ+z];
    else
        return EMPTY;
}

void Chunk::setBlockType(int x, int y, int z, BlockType type)
{
    if(x>=0&&x<MaxX&&y>=0&&y<MaxY&&z>=0&&z<MaxZ)
        mBlock[x*MaxY*MaxZ+y*MaxZ+z] = type;
}
