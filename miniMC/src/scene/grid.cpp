#include "grid.h"

Grid::Grid()
{
    for(int i = 0;i<MaxU*MaxV;i++)
    {
        grid[i] = false;
    }
}

//mirror
bool Grid::getValue(glm::ivec2 pos)
{
    int posX = pos.x < 0 ? -pos.x : pos.x;
    int posY = pos.y < 0 ? -pos.y : pos.y;

    if(posX>=0&&posX<MaxU&&posY>=0&&posY<MaxV)
    {
        return grid[posX*MaxV + posY];
    }
    else
    {
        return false;
    }
}

void Grid::setValue(glm::ivec2 pos, bool value)
{
    if(pos.x>=0&&pos.x<MaxU&&pos.y>=0&&pos.y<MaxV)
    {
        grid[pos.x*MaxV + pos.y] = value;
    }
}

void Grid::setLineValue(glm::vec2 posFrom, glm::vec2 posTo, bool value, float width)
{
    //glm::vec2 widthNor = glm::normalize(widthDir);
    glm::vec2 a = posTo - posFrom;
    glm::vec2 b(0,0);
    if(a.x!=0) b = glm::vec2(-a.y/a.x,1);
    else if(a.y!=0) b = glm::vec2(1,-a.x/a.y);
    else return;

    glm::vec2 widthNor = glm::normalize(b);
    for(float i = 0;i<width;i+=0.2)
    {
        glm::vec2 tempFrom = posFrom + widthNor*i;
        glm::vec2 tempTo = posTo + widthNor*i;

        glm::vec2 tempCur = tempFrom;
        glm::vec2 direction = glm::normalize(tempTo-tempFrom);
        while(glm::length(tempCur-tempFrom) < glm::length(tempTo-tempFrom))
        {
            glm::ivec2 tempCurInt((int)tempCur.x,(int)tempCur.y);
            setValue(tempCurInt, value);
            tempCur += direction;
        }
    }
}

void Grid::printAll()
{
    for(int i = 0;i<MaxU;i++)
    {
        for(int k = 0;k<MaxV;k++)
        {
            printf("%d",grid[i*MaxV+k]);
        }
        printf("\n");
    }
    printf("\n");
}
