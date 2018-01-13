#ifndef CHUNK_H
#define CHUNK_H

#include "drawable.h"
#include "scene/terrain.h"
#include "utils.h"

#define MaxX 48
#define MaxY 32
#define MaxZ 48

class Terrain;

class Chunk : public Drawable
{
private:
    glm::vec4 getBlockTypeColor(BlockType bt);
    glm::vec2 getBlockTypeUV(BlockType bt, int face = 0);
    glm::vec2 getBlockTypeAnim(BlockType bt);
    void createSquare(int i, int j, int k,
                      std::vector<glm::vec4> &posV,
                      std::vector<glm::vec4> &norV,
                      std::vector<glm::vec4> &colV,
                      std::vector<GLuint> &idxV);
    void createSquareEX(int i, int j, int k,
                      std::vector<glm::vec4> &posV,
                      std::vector<glm::vec4> &norV,
                      std::vector<glm::vec4> &colV,
                      std::vector<glm::vec2> &uvV,
                      std::vector<glm::vec2> &animV,
                      std::vector<GLuint> &idxV);
    BlockType mBlock[MaxX*MaxY*MaxZ];
    std::vector<glm::vec4> tempPosV;
    std::vector<glm::vec4> tempNorV;
    std::vector<glm::vec4> tempColV;
    std::vector<glm::vec2> tempUvV;
    std::vector<glm::vec2> tempAnimV;
    std::vector<GLuint> tempIdxV;
public:
    glm::ivec2 mPos;
    Terrain* mpTerrain;
    Chunk(OpenGLContext* context, glm::ivec2 pos = glm::ivec2(0,0));
    ~Chunk();
    void destroy();
    void create();//three buffer for pos, nor, col
    void createEX();//one buffer for pos, nor, col, check adjacent chunk
    void createEXX();//three buffer for pos, nor, col, check adjacent chunk
    void createEXXX();//no bind
    void bind();
    BlockType getBlockType(int x, int y, int z);
    void setBlockType(int x, int y, int z, BlockType type);
};

#endif // CHUNK_H
