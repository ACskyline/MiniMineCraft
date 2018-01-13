#include "chunk.h"

Chunk::Chunk(OpenGLContext* context, glm::ivec2 pos) :
    Drawable(context), mBlock{EMPTY}, mPos(pos)
{
    for(int i = 0;i<MaxX*MaxY*MaxZ;i++)
    {
        mBlock[i] = EMPTY;
    }
}

Chunk::~Chunk()
{
    destroy();
}

void Chunk::destroy()
{
    tempPosV.clear();
    tempNorV.clear();
    tempColV.clear();
    tempUvV.clear();
    tempAnimV.clear();
    tempIdxV.clear();
    //Drawable::destroy();
}

glm::vec2 Chunk::getBlockTypeAnim(BlockType bt)
{
    glm::vec2 anim(0,0);
    switch(bt)
    {
    case LAVA:
    case WATER:
        anim = glm::vec2(1, 1);
        break;
    case GRASS:
    case DIRT:
    case STONE:
    case WOOD:
    case LEAF:
    case BEDROCK:
    case ICE:
    case EMPTY:
    default:
        anim = glm::vec2(0,0);
        break;
    }
    return anim;
}

glm::vec2 Chunk::getBlockTypeUV(BlockType bt, int face)
{
    glm::vec2 uv(1,1);
    switch(bt)
    {
    case GRASS:
        switch(face)
        {
        case 0:
            uv = glm::vec2(32.f/256.f, 241.f/256.f);
            break;
        case 1:
            uv = glm::vec2(128.f/256.f, 209.f/256.f);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            uv = glm::vec2(48.f/256.f, 241.f/256.f);
            break;
        default:
            break;
        }
        break;
    case DIRT:
        uv = glm::vec2(32.f/256.f, 241.f/256.f);
        break;
    case STONE:
        uv = glm::vec2(16.f/256.f, 241.f/256.f);
        break;
    case WOOD:
        switch(face)
        {
        case 0:
        case 1:
            uv = glm::vec2(80.f/256.f, 225.f/256.f);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            uv = glm::vec2(64.f/256.f, 225.f/256.f);
            break;
        default:
            break;
        }
        break;
    case LEAF:
        uv = glm::vec2(80.f/256.f, 193.f/256.f);
        break;
    case BEDROCK:
        uv = glm::vec2(16.f/256.f, 225.f/256.f);
        break;
    case LAVA:
        uv = glm::vec2(224.f/256.f, 17.f/256.f);
        break;
    case WATER:
        uv = glm::vec2(224.f/256.f, 33.f/256.f);
        break;
    case ICE:
        uv = glm::vec2(48.f/256.f, 177.f/256.f);
        break;
    case EMPTY:
    default:
        uv = glm::vec2(0,0);
        break;
    }
    return uv;
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
                           std::vector<glm::vec2> &uvV,
                           std::vector<glm::vec2> &animV,
                           std::vector<GLuint> &idxV)
{
    if(mpTerrain==nullptr||mpTerrain->getBlockAt(i,j,k) == EMPTY)
    {
        return;
    }

    BlockType lBlockType = mpTerrain->getBlockAt(i,j,k);

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

    BlockType bt = getBlockType(blockI,blockJ,blockK);
    glm::vec4 col = getBlockTypeColor(bt);
    glm::vec2 anim = getBlockTypeAnim(bt);


    //    printf("%d,%d,%d:%d ",i-1,j,k,mpTerrain->getBlockAt(i-1,j,k));
    //    printf("%d,%d,%d:%d ",i+1,j,k,mpTerrain->getBlockAt(i+1,j,k));
    //    printf("%d,%d,%d:%d ",i,j-1,k,mpTerrain->getBlockAt(i,j-1,k));
    //    printf("%d,%d,%d:%d ",i,j+1,k,mpTerrain->getBlockAt(i,j+1,k));
    //    printf("%d,%d,%d:%d ",i,j,k-1,mpTerrain->getBlockAt(i,j,k-1));
    //    printf("%d,%d,%d:%d ",i,j,k+1,mpTerrain->getBlockAt(i,j,k+1));


    //-x direction
    BlockType xN = mpTerrain->getBlockAt(i-1,j,k);
    BlockType xNextra = mpTerrain->mExtraTerrain->getBlockAt(i-1,j,k);
    if((mpTerrain->mType==1&&xN==EMPTY&&xNextra==EMPTY)||(mpTerrain->mType==0&&xN==EMPTY))
        //    if((xN==EMPTY&&mpTerrain->mExtraTerrain!=nullptr&&xNextra == EMPTY) ||
        //        (xN==EMPTY&&mpTerrain->mExtraTerrain==nullptr))
    {
        glm::vec2 uv0 = getBlockTypeUV(bt,5);
        glm::vec2 uv1 = uv0 + glm::vec2(15.f/256.f, 0);
        glm::vec2 uv2 = uv0 + glm::vec2(15.f/256.f, 15.f/256.f);
        glm::vec2 uv3 = uv0 + glm::vec2(0, 15.f/256.f);

        uvV.push_back(uv0);
        uvV.push_back(uv1);
        uvV.push_back(uv2);
        uvV.push_back(uv3);

        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos3);
        posV.push_back(pos7);
        posV.push_back(pos4);

        for(int a = 0;a<4;++a)
        {
            norV.push_back(nor5);
            colV.push_back(col);
            animV.push_back(anim);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+x direction
    BlockType xP = mpTerrain->getBlockAt(i+1,j,k);
    BlockType xPextra = mpTerrain->mExtraTerrain->getBlockAt(i+1,j,k);
    if((mpTerrain->mType==1&&xP==EMPTY&&xPextra==EMPTY)||(mpTerrain->mType==0&&xP==EMPTY))
        //    if((xP==EMPTY&&mpTerrain->mExtraTerrain!=nullptr&&mpTerrain->mExtraTerrain->getBlockAt(i+1,j,k) == EMPTY) ||
        //        (xP==EMPTY&&mpTerrain->mExtraTerrain==nullptr))
    {
        glm::vec2 uv0 = getBlockTypeUV(bt,3);
        glm::vec2 uv1 = uv0 + glm::vec2(15.f/256.f, 0);
        glm::vec2 uv2 = uv0 + glm::vec2(15.f/256.f, 15.f/256.f);
        glm::vec2 uv3 = uv0 + glm::vec2(0, 15.f/256.f);

        uvV.push_back(uv0);
        uvV.push_back(uv1);
        uvV.push_back(uv2);
        uvV.push_back(uv3);

        int idstart = posV.size();

        posV.push_back(pos2);
        posV.push_back(pos1);
        posV.push_back(pos5);
        posV.push_back(pos6);

        for(int a = 0;a<4;++a)
        {
            norV.push_back(nor3);
            colV.push_back(col);
            animV.push_back(anim);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //-y direction
    BlockType yN = mpTerrain->getBlockAt(i,j-1,k);
    BlockType yNextra = mpTerrain->mExtraTerrain->getBlockAt(i,j-1,k);
    if((mpTerrain->mType==1&&yN==EMPTY&&yNextra==EMPTY)||(mpTerrain->mType==0&&yN==EMPTY))
        //    if((yN==EMPTY&&mpTerrain->mExtraTerrain!=nullptr&&mpTerrain->mExtraTerrain->getBlockAt(i,j-1,k) == EMPTY) ||
        //        (yN==EMPTY&&mpTerrain->mExtraTerrain==nullptr))
    {
        glm::vec2 uv0 = getBlockTypeUV(bt,0);
        glm::vec2 uv1 = uv0 + glm::vec2(15.f/256.f, 0);
        glm::vec2 uv2 = uv0 + glm::vec2(15.f/256.f, 15.f/256.f);
        glm::vec2 uv3 = uv0 + glm::vec2(0, 15.f/256.f);

        uvV.push_back(uv0);
        uvV.push_back(uv1);
        uvV.push_back(uv2);
        uvV.push_back(uv3);

        int idstart = posV.size();

        posV.push_back(pos0);
        posV.push_back(pos1);
        posV.push_back(pos2);
        posV.push_back(pos3);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor0);
            colV.push_back(col);
            animV.push_back(anim);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+y direction
    BlockType yP = mpTerrain->getBlockAt(i,j+1,k);
    BlockType yPextra = mpTerrain->mExtraTerrain->getBlockAt(i,j+1,k);
    if((mpTerrain->mType==1&&yP==EMPTY&&yPextra==EMPTY)||(mpTerrain->mType==0&&yP==EMPTY))
        //    if((yP==EMPTY&&mpTerrain->mExtraTerrain!=nullptr&&mpTerrain->mExtraTerrain->getBlockAt(i,j+1,k) == EMPTY) ||
        //        (yP==EMPTY&&mpTerrain->mExtraTerrain==nullptr))
    {
        glm::vec2 uv0 = getBlockTypeUV(bt,1);
        glm::vec2 uv1 = uv0 + glm::vec2(15.f/256.f, 0);
        glm::vec2 uv2 = uv0 + glm::vec2(15.f/256.f, 15.f/256.f);
        glm::vec2 uv3 = uv0 + glm::vec2(0, 15.f/256.f);

        uvV.push_back(uv0);
        uvV.push_back(uv1);
        uvV.push_back(uv2);
        uvV.push_back(uv3);

        int idstart = posV.size();

        posV.push_back(pos5);
        posV.push_back(pos4);
        posV.push_back(pos7);
        posV.push_back(pos6);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor1);
            colV.push_back(col);
            animV.push_back(anim);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //-z direction
    BlockType zN = mpTerrain->getBlockAt(i,j,k-1);
    BlockType zNextra = mpTerrain->mExtraTerrain->getBlockAt(i,j,k-1);
    if((mpTerrain->mType==1&&zN==EMPTY&&zNextra==EMPTY)||(mpTerrain->mType==0&&zN==EMPTY))
        //    if((zN==EMPTY&&mpTerrain->mExtraTerrain!=nullptr&&mpTerrain->mExtraTerrain->getBlockAt(i,j,k-1) == EMPTY) ||
        //        (zN==EMPTY&&mpTerrain->mExtraTerrain==nullptr))
    {
        glm::vec2 uv0 = getBlockTypeUV(bt,2);
        glm::vec2 uv1 = uv0 + glm::vec2(15.f/256.f, 0);
        glm::vec2 uv2 = uv0 + glm::vec2(15.f/256.f, 15.f/256.f);
        glm::vec2 uv3 = uv0 + glm::vec2(0, 15.f/256.f);

        uvV.push_back(uv0);
        uvV.push_back(uv1);
        uvV.push_back(uv2);
        uvV.push_back(uv3);

        int idstart = posV.size();

        posV.push_back(pos1);
        posV.push_back(pos0);
        posV.push_back(pos4);
        posV.push_back(pos5);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor2);
            colV.push_back(col);
            animV.push_back(anim);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
    //+z direction
    BlockType zP = mpTerrain->getBlockAt(i,j,k+1);
    BlockType zPextra = mpTerrain->mExtraTerrain->getBlockAt(i,j,k+1);
    if((mpTerrain->mType==1&&zP==EMPTY&&zPextra==EMPTY)||(mpTerrain->mType==0&&zP==EMPTY))
        //    if((zP==EMPTY&&mpTerrain->mExtraTerrain!=nullptr&&mpTerrain->mExtraTerrain->getBlockAt(i,j,k+1) == EMPTY) ||
        //        (zP==EMPTY&&mpTerrain->mExtraTerrain==nullptr))
    {
        glm::vec2 uv0 = getBlockTypeUV(bt,4);
        glm::vec2 uv1 = uv0 + glm::vec2(15.f/256.f, 0);
        glm::vec2 uv2 = uv0 + glm::vec2(15.f/256.f, 15.f/256.f);
        glm::vec2 uv3 = uv0 + glm::vec2(0, 15.f/256.f);

        uvV.push_back(uv0);
        uvV.push_back(uv1);
        uvV.push_back(uv2);
        uvV.push_back(uv3);

        int idstart = posV.size();

        posV.push_back(pos3);
        posV.push_back(pos2);
        posV.push_back(pos6);
        posV.push_back(pos7);

        for(int a = 0;a<4;a++)
        {
            norV.push_back(nor4);
            colV.push_back(col);
            animV.push_back(anim);
            if(a>2)
            {
                idxV.push_back(idstart);
                idxV.push_back(idstart+a-1);
            }
            idxV.push_back(idstart+a);
        }
    }
}

//independent VBO, not check adjacent faces
void Chunk::create()
{
    destroy();

    //    std::vector<glm::vec4> tempPosV;
    //    std::vector<glm::vec4> tempNorV;
    //    std::vector<glm::vec4> tempColV;

    //    std::vector<GLuint> tempIdxV;

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

//independent VBO, check adjacent faces
void Chunk::createEXX()
{
    destroy();

    //    std::vector<glm::vec4> tempPosV;
    //    std::vector<glm::vec4> tempNorV;
    //    std::vector<glm::vec4> tempColV;
    //    std::vector<glm::vec2> tempUvV;
    //    std::vector<glm::vec2> tempAnimV;

    //    std::vector<GLuint> tempIdxV;

    for(int i = mPos.x;i<mPos.x + MaxX;i++)
    {
        for(int j = 0;j<MaxY;j++)
        {
            for(int k = mPos.y;k<mPos.y + MaxZ;k++)
            {
                createSquareEX(i,j,k,tempPosV,tempNorV,tempColV,tempUvV,tempAnimV,tempIdxV);
            }
        }
    }

    //printf("size:%d,%d,%d,%d ",tempIdxV.size(),tempPosV.size(),tempNorV.size(),tempColV.size());


    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    if(tempPosV.size()>0)
    {
        generatePos();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
        context->glBufferData(GL_ARRAY_BUFFER, tempPosV.size() * sizeof(glm::vec4), tempPosV.data(), GL_STATIC_DRAW);
    }
    if(tempNorV.size()>0)
    {
        generateNor();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
        context->glBufferData(GL_ARRAY_BUFFER, tempNorV.size() * sizeof(glm::vec4), tempNorV.data(), GL_STATIC_DRAW);
    }

    if(tempColV.size()>0)
    {
        generateCol();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
        context->glBufferData(GL_ARRAY_BUFFER, tempColV.size() * sizeof(glm::vec4), tempColV.data(), GL_STATIC_DRAW);
    }

    if(tempUvV.size()>0)
    {
        generateUV();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);
        context->glBufferData(GL_ARRAY_BUFFER, tempUvV.size() * sizeof(glm::vec2), tempUvV.data(), GL_STATIC_DRAW);
    }
    if(tempAnimV.size()>0)
    {
        generateAnim();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufAnim);
        context->glBufferData(GL_ARRAY_BUFFER, tempAnimV.size() * sizeof(glm::vec2), tempAnimV.data(), GL_STATIC_DRAW);
    }

    if(tempIdxV.size()>0)
    {
        // Create a VBO on our GPU and store its handle in bufIdx
        generateIdx();
        // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
        // and that it will be treated as an element array buffer (since it will contain triangle indices)
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
        // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
        // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
        context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempIdxV.size() * sizeof(GLuint), tempIdxV.data(), GL_STATIC_DRAW);
    }
}

void Chunk::createEXXX()
{
    destroy();

    for(int i = mPos.x;i<mPos.x + MaxX;i++)
    {
        for(int j = 0;j<MaxY;j++)
        {
            for(int k = mPos.y;k<mPos.y + MaxZ;k++)
            {
                createSquareEX(i,j,k,tempPosV,tempNorV,tempColV,tempUvV,tempAnimV,tempIdxV);
            }
        }
    }
}

void Chunk::bind()
{
    if(tempIdxV.size()>0)
    {
        // Create a VBO on our GPU and store its handle in bufIdx
        if(!idxBound)
            generateIdx();
        // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
        // and that it will be treated as an element array buffer (since it will contain triangle indices)
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
        // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
        // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
        context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempIdxV.size() * sizeof(GLuint), tempIdxV.data(), GL_STATIC_DRAW);
    }
    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    if(tempPosV.size()>0)
    {
        if(!posBound)
            generatePos();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
        context->glBufferData(GL_ARRAY_BUFFER, tempPosV.size() * sizeof(glm::vec4), tempPosV.data(), GL_STATIC_DRAW);
    }
    if(tempNorV.size()>0)
    {
        if(!norBound)
            generateNor();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
        context->glBufferData(GL_ARRAY_BUFFER, tempNorV.size() * sizeof(glm::vec4), tempNorV.data(), GL_STATIC_DRAW);
    }

    if(tempColV.size()>0)
    {
        if(!colBound)
            generateCol();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
        context->glBufferData(GL_ARRAY_BUFFER, tempColV.size() * sizeof(glm::vec4), tempColV.data(), GL_STATIC_DRAW);
    }

    if(tempUvV.size()>0)
    {
        if(!uvBound)
            generateUV();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);
        context->glBufferData(GL_ARRAY_BUFFER, tempUvV.size() * sizeof(glm::vec2), tempUvV.data(), GL_STATIC_DRAW);
    }
    if(tempAnimV.size()>0)
    {
        if(!animBound)
            generateAnim();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufAnim);
        context->glBufferData(GL_ARRAY_BUFFER, tempAnimV.size() * sizeof(glm::vec2), tempAnimV.data(), GL_STATIC_DRAW);
    }
}

//combined VBO, check adjacent faces
void Chunk::createEX()
{
    destroy();

    //    std::vector<glm::vec4> tempPosV;
    //    std::vector<glm::vec4> tempNorV;
    //    std::vector<glm::vec4> tempColV;
    //    std::vector<glm::vec2> tempUvV;
    //    std::vector<glm::vec2> tempAnimV;
        std::vector<glm::vec4> tempCombineV;

    //    std::vector<GLuint> tempIdxV;

    for(int i = mPos.x;i<mPos.x + MaxX;i++)
    {
        for(int j = 0;j<MaxY;j++)
        {
            for(int k = mPos.y;k<mPos.y + MaxZ;k++)
            {
                createSquareEX(i,j,k,tempPosV,tempNorV,tempColV,tempUvV,tempAnimV,tempIdxV);
            }
        }
    }

    for(int i = 0;i<tempPosV.size();i++)
    {
        tempCombineV.push_back(tempPosV.at(i));
        tempCombineV.push_back(tempNorV.at(i));
        tempCombineV.push_back(tempColV.at(i));
        //printf("pos:%f,%f,%f,%f ",tempPosV.at(i)[0],tempPosV.at(i)[1],tempPosV.at(i)[2],tempPosV.at(i)[3]);
        //printf("nor:%f,%f,%f,%f ",tempNorV.at(i)[0],tempNorV.at(i)[1],tempNorV.at(i)[2],tempNorV.at(i)[3]);
        //printf("col:%f,%f,%f,%f\n",tempColV.at(i)[0],tempColV.at(i)[1],tempColV.at(i)[2],tempColV.at(i)[3]);
    }
    //printf("size:%d,%d,%d,%d,%d ",tempIdxV.size(),tempPosV.size(),tempNorV.size(),tempColV.size(),tempCombineV.size());

    //    for(int i = 0;i<tempIdxV.size();i++)
    //    {
    //        printf("index:%d ",tempIdxV[i]);
    //    }

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
