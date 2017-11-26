#pragma once
#include <QList>
#include <la.h>
#include "chunk.h"
#include "player.h"
#include "utils.h"

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.

class Player;

class Chunk;

class Terrain
{
private:
    glm::vec3 mOrigin; //not used yet
public:

    Terrain(OpenGLContext* context, glm::vec3 origin = glm::vec3(0,0,0));
    std::vector<Chunk*> mChunkVector;
    std::map<std::pair<int,int>,Chunk*> mChunkMap;
    OpenGLContext* mContext;
    //BlockType m_blocks[64][256][64];                    // A 3D list of the blocks in the world.
                                                           // You'll need to replace this with a far more
                                                           // efficient system of storing terrain.
    void createTestChunk(glm::ivec2 origin = glm::ivec2(0,0));
    void createChunk(glm::ivec2 origin = glm::ivec2(0,0));
    void createChunkEX(glm::ivec2 origin = glm::ivec2(0,0));
    //glm::ivec3 dimensions;

    BlockType getBlockAt(int x, int y, int z);   // Given a world-space coordinate (which may have negative
                                                           // values) return the block stored at that point in space.
    void setBlockAt(int x, int y, int z, BlockType t); // Given a world-space coordinate (which may have negative
                                                           // values) set the block at that point in space to the
                                                           // given type.

    Chunk* getChunkAt(int x, int z);

    BlockType getBlockAtEX(glm::vec3 pos);
    void setBlockAtEX(glm::vec3 pos, BlockType t);
    Chunk* getChunkAtEX(glm::vec3 pos);

    ~Terrain();
    glm::ivec2 toChunkXZ(glm::vec3 pos);
    glm::ivec3 toBlockXYZ(glm::vec3 pos);
    float noise(glm::ivec2 p);
    float random(glm::vec2 n);
    float interpNoise(glm::vec2 p);
    float fbm(glm::vec2 p);
    float pattern(glm::vec2 p);
};
