#include <scene/terrain.h>

#include <scene/cube.h>
#include <utility>

Terrain::Terrain(OpenGLContext* context, glm::vec3 origin) : mOrigin(origin), mContext(context)//, dimensions(64, 256, 64)
{}

Terrain::~Terrain()
{
    for(int i = 0;i<mChunkVector.size();i++)
    {
        delete mChunkVector.at(i);
    }
    mChunkVector.clear();
    mChunkMap.clear();
}

BlockType Terrain::getBlockAt(int x, int y, int z)
{
    // TODO: Make this work with your new block storage!

    float X = (float)x/MaxX;
    float Z = (float)z/MaxZ;

    int xInt = glm::floor(X);//= X<0 ? (int)(X-1) : (int)X;//floor //what I wrote is not floor!!!
    int zInt = glm::floor(Z);//= Z<0 ? (int)(Z-1) : (int)Z;//floor

    int chunkX = xInt * MaxX;
    int chunkZ = zInt * MaxZ;

    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    if(mChunkMap.count(std::make_pair(chunkX,chunkZ))!=0)
    {
        Chunk* chunk = mChunkMap[std::make_pair(chunkX,chunkZ)];
        if(chunk!=nullptr)
            return chunk->getBlockType(blockX,y,blockZ);
        else
            return EMPTY;
    }
    else
    {
        return EMPTY;
    }
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    // TODO: Make this work with your new block storage!
    float X = (float)x/MaxX;
    float Z = (float)z/MaxZ;

    int xInt = glm::floor(X);//= X<0 ? (int)(X-1) : (int)X;//floor //what I wrote is not floor!!!
    int zInt = glm::floor(Z);//= Z<0 ? (int)(Z-1) : (int)Z;//floor

    int chunkX = xInt * MaxX;
    int chunkZ = zInt * MaxZ;

    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    if(mChunkMap.count(std::make_pair(chunkX,chunkZ))!=0)
        mChunkMap[std::make_pair(chunkX,chunkZ)]->setBlockType(blockX,y,blockZ,t);
}

void Terrain::setBlockAtEX(glm::vec3 pos, BlockType t)
{
    glm::ivec3 block = toBlockXYZ(pos);
    int blockX = block.x;
    int blockY = block.y;
    int blockZ = block.z;

    glm::ivec2 chunkk = toChunkXZ(pos);
    int chunkX = chunkk.x;
    int chunkZ = chunkk.y;

    if(mChunkMap.count(std::make_pair(chunkX,chunkZ))!=0)
    {
        Chunk* chunk = mChunkMap[std::make_pair(chunkX,chunkZ)];
        if(chunk!=nullptr)
            chunk->setBlockType(blockX,blockY,blockZ,t);
    }
}

BlockType Terrain::getBlockAtEX(glm::vec3 pos)
{
    glm::ivec3 block = toBlockXYZ(pos);
    int blockX = block.x;
    int blockY = block.y;
    int blockZ = block.z;

    glm::ivec2 chunkk = toChunkXZ(pos);
    int chunkX = chunkk.x;
    int chunkZ = chunkk.y;

    if(mChunkMap.count(std::make_pair(chunkX,chunkZ))!=0)
    {
        Chunk* chunk = mChunkMap[std::make_pair(chunkX,chunkZ)];
        if(chunk!=nullptr)
            return chunk->getBlockType(blockX,blockY,blockZ);
        else
            return EMPTY;
    }
    else
    {
        return EMPTY;
    }
}


Chunk* Terrain::getChunkAt(int x, int z)
{
    if(mChunkMap.count(std::make_pair(x,z))!=0)
    {
        Chunk* chunk = mChunkMap[std::make_pair(x,z)];
        return chunk;
    }
    else
    {
        return nullptr;
    }
}

Chunk* Terrain::getChunkAtEX(glm::vec3 pos)
{
    glm::ivec2 chunkk = toChunkXZ(pos);
    int chunkX = chunkk.x;
    int chunkZ = chunkk.y;

    if(mChunkMap.count(std::make_pair(chunkX,chunkZ))!=0)
    {
        Chunk* chunk = mChunkMap[std::make_pair(chunkX,chunkZ)];
        return chunk;
    }
    else
    {
        return nullptr;
    }
}


glm::ivec3 Terrain::toBlockXYZ(glm::vec3 pos)
{
    glm::ivec3 result;

    int xInt = glm::floor(pos.x);//pos.x<0 ? (int)(pos.x-1) : (int)pos.x;//floor //what I wrote is not floor!!!
    int yInt = glm::floor(pos.y);//pos.y<0 ? (int)(pos.y-1) : (int)pos.y;//floor
    int zInt = glm::floor(pos.z);//pos.z<0 ? (int)(pos.z-1) : (int)pos.z;//floor

    glm::ivec2 chunkXZ = toChunkXZ(pos);
    result.x = xInt - chunkXZ.x;
    result.y = yInt;
    result.z = zInt - chunkXZ.y;

    return result;
}

glm::ivec2 Terrain::toChunkXZ(glm::vec3 pos)
{
    glm::ivec2 result;

    float X = pos.x/MaxX;
    float Z = pos.z/MaxZ;


    int xInt = glm::floor(X);//= X<0 ? (int)(X-1) : (int)X;//floor //what I wrote is not floor!!!
    int zInt = glm::floor(Z);//= Z<0 ? (int)(Z-1) : (int)Z;//floor

    result.x = xInt * MaxX;
    result.y = zInt * MaxZ;

    return result;
}

float Terrain::noise(glm::ivec2 p)
{
    p.x = (p.x<<13) ^ p.x;
    p.y = (p.y<<13) ^ p.y;
    return (1 - (p.x*p.y*(p.x*p.y*15731 + 789221) + 1376312589) & 0x7fffffff)/10737741824.0;
}

float Terrain::random(glm::vec2 n)
{
    return (glm::fract(glm::sin(glm::dot(n, glm::vec2(12.9898, 4.1414))) * 43758.5453));
}

float Terrain::interpNoise(glm::vec2 p)
{
    float intX = glm::floor(p.x);
    float fractX = glm::fract(p.x);
    float intY = glm::floor(p.y);
    float fractY = glm::fract(p.y);

    //    float v1 = noise(glm::ivec2(intX, intY));
    //    float v2 = noise(glm::ivec2(intX+1, intY));
    //    float v3 = noise(glm::ivec2(intX, intY+1));
    //    float v4 = noise(glm::ivec2(intX+1, intY+1));

    float v1 = random(glm::vec2(intX, intY));
    float v2 = random(glm::vec2(intX+1, intY));
    float v3 = random(glm::vec2(intX, intY+1));
    float v4 = random(glm::vec2(intX+1, intY+1));

    float i1 = glm::mix(v1, v2, fractX);
    float i2 = glm::mix(v3, v4, fractX);

    return glm::mix(i1, i2, fractY);
}

float Terrain::fbm(glm::vec2 p)
{
    float total = 0;
    float presistence = 0.5;
    int octaves = 8;

    for(int i =0;i<octaves;i++)
    {
        float freq = pow(2.f, i);
        float amp = pow(presistence, i);
        total += interpNoise(p * freq) * amp;
    }
    return total;
}

float Terrain::pattern(glm::vec2 p)
{
    glm::vec2 q = glm::vec2( fbm(p + glm::vec2(0, 0)),
                             fbm(p + glm::vec2(5.2, 1.3)));

    glm::vec2 r = glm::vec2( fbm(p + 4.f*q + glm::vec2(1.7, 9.2)),
                             fbm(p + 4.f*q + glm::vec2(8.3, 2.8)));

    return fbm(p+4.f*r);
}

void Terrain::createTestChunk(glm::ivec2 origin)
{
    Chunk* newChunk = new Chunk(mContext, origin);
    // Create the basic terrain floor
    for(int x = 0; x < MaxX; ++x)
    {
        for(int z = 0; z < MaxZ; ++z)
        {
            for(int y = 0; y < MaxY/2; ++y)
            {
                if((x + z) % 2 == 0)
                {
                    newChunk->setBlockType(x,y,z,STONE);
                }
                else
                {
                    newChunk->setBlockType(x,y,z,DIRT);
                }
            }
        }
    }
    // Add "walls" for collision testing
    for(int x = 0; x < MaxX; ++x)
    {
        newChunk->setBlockType(x,MaxY/2,0,GRASS);
        newChunk->setBlockType(x,MaxY/2+1,0,GRASS);
        newChunk->setBlockType(x,MaxY/2,MaxZ-1,GRASS);
        newChunk->setBlockType(0,MaxY/2+1,x,GRASS);
    }
    for(int y = MaxY/2; y < MaxY/2+10; ++y)
    {
        newChunk->setBlockType(MaxX/2,y,MaxZ/2,GRASS);
    }

    newChunk->mpTerrain = this;
    mChunkVector.push_back(newChunk);
    mChunkMap[std::make_pair(newChunk->mPos.x,newChunk->mPos.y)] = newChunk;
    newChunk->createEXX();////////////////////////////////////////////////////////////

    glm::ivec2 offset[4] = { glm::ivec2(-MaxX,0),
                             glm::ivec2(MaxX,0),
                             glm::ivec2(0,-MaxZ),
                             glm::ivec2(0,MaxZ) };
    for(int m=0;m<4;m++)
    {
        Chunk* oldChunk = nullptr;
        if(mChunkMap.count(std::make_pair(origin.x+offset[m].x,origin.y+offset[m].y))!=0)
            oldChunk = mChunkMap[std::make_pair(origin.x+offset[m].x,origin.y+offset[m].y)];
        if(oldChunk!=nullptr)
            oldChunk->createEXX();////////////////////////////////////////////////////////////
    }
}

void Terrain::createChunk(glm::ivec2 origin)
{
    Chunk* newChunk = new Chunk(mContext, origin);
    for(int i = 0;i<MaxX;i++)
    {
        for(int k = 0;k<MaxZ;k++)
        {
            float yF = fbm(glm::vec2((origin.x+(float)i)/(MaxX),(origin.y+(float)k)/MaxY));
            int yI = 2+(int)(yF*10.f);
            for(int j = 0;j<MaxY/2+yI;j++)
            {
                if(j<MaxY/2)
                {
                    newChunk->setBlockType(i,j,k,STONE);
                }
                else if(j<MaxY/2+yI-1)
                {
                    newChunk->setBlockType(i,j,k,DIRT);
                }
                else
                {
                    newChunk->setBlockType(i,j,k,GRASS);
                }
            }
        }
    }
    //
    newChunk->mpTerrain = this;
    mChunkVector.push_back(newChunk);
    mChunkMap[std::make_pair(newChunk->mPos.x,newChunk->mPos.y)] = newChunk;
    newChunk->createEXX();

    glm::ivec2 offset[4] = { glm::ivec2(-MaxX,0),
                             glm::ivec2(MaxX,0),
                             glm::ivec2(0,-MaxZ),
                             glm::ivec2(0,MaxZ) };
    for(int m=0;m<4;m++)
    {
        Chunk* oldChunk = nullptr;
        if(mChunkMap.count(std::make_pair(origin.x+offset[m].x,origin.y+offset[m].y))!=0)
            oldChunk = mChunkMap[std::make_pair(origin.x+offset[m].x,origin.y+offset[m].y)];
        if(oldChunk!=nullptr)
            oldChunk->createEXX();
    }
}

void Terrain::createChunkEX(glm::ivec2 origin)
{
    Chunk* newChunk = new Chunk(mContext, origin);
    for(int i = 0;i<MaxX;i++)
    {
        for(int k = 0;k<MaxZ;k++)
        {
            float yF = fbm(glm::vec2((origin.x+(float)i)/(MaxX),(origin.y+(float)k)/MaxY));
            int yI = 2+(int)(yF*10.f);
            for(int j = 0;j<MaxY/2+yI;j++)
            {
                if(j<MaxY/2)
                {
                    newChunk->setBlockType(i,j,k,STONE);
                }
                else if(j<MaxY/2+yI-1)
                {
                    newChunk->setBlockType(i,j,k,DIRT);
                }
                else
                {
                    newChunk->setBlockType(i,j,k,GRASS);
                }
            }
        }
    }

    newChunk->mpTerrain = this;
    mChunkVector.push_back(newChunk);
    mChunkMap[std::make_pair(newChunk->mPos.x,newChunk->mPos.y)] = newChunk;
    newChunk->createEXX();

    glm::ivec2 offset[4] = { glm::ivec2(-MaxX,0),
                             glm::ivec2(MaxX,0),
                             glm::ivec2(0,-MaxZ),
                             glm::ivec2(0,MaxZ) };
    for(int m=0;m<4;m++)
    {
        Chunk* oldChunk = nullptr;
        if(mChunkMap.count(std::make_pair(origin.x+offset[m].x,origin.y+offset[m].y))!=0)
            oldChunk = mChunkMap[std::make_pair(origin.x+offset[m].x,origin.y+offset[m].y)];
        if(oldChunk!=nullptr)
            oldChunk->createEXX();
    }
}
