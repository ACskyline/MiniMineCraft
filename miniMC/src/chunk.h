#ifndef CHUNK_H
#define CHUNK_H

#include "drawable.h"
#include "scene/terrain.h"
#include "utils.h"

#define MaxX 16
#define MaxY 256
#define MaxZ 16

class Terrain;

class Chunk : public Drawable
{
private:
    glm::vec4 getBlockTypeColor(BlockType bt);
    void createSquare(int i, int j, int k,
                      std::vector<glm::vec4> &posV,
                      std::vector<glm::vec4> &norV,
                      std::vector<glm::vec4> &colV,
                      std::vector<GLuint> &idxV);
    void createSquareEX(int i, int j, int k,
                      std::vector<glm::vec4> &posV,
                      std::vector<glm::vec4> &norV,
                      std::vector<glm::vec4> &colV,
                      std::vector<GLuint> &idxV);
    BlockType mBlock[MaxX*MaxY*MaxZ];
public:
    glm::ivec2 mPos;
    Terrain* mpTerrain;
    Chunk(OpenGLContext* context, glm::ivec2 pos = glm::ivec2(0,0));
    ~Chunk();
    void create();//three buffer for pos, nor, col
    void createEX();//three buffer for pos, nor, col, check adjacent chunk
    void createEXX();//one buffer for pos, nor, col, check adjacent chunk
    BlockType getBlockType(int x, int y, int z);
    void setBlockType(int x, int y, int z, BlockType type);
};

#endif // CHUNK_H
