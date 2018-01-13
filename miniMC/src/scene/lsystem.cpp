#include "lsystem.h"
#include <QTime>

LSystem::LSystem()
{
    srand(QDateTime::currentMSecsSinceEpoch());
}

LSystem::~LSystem()
{
    destroy();
}

void LSystem::destroy()
{
    for(int i = 0;i<turtleV.size();i++)
    {
        delete turtleV[i];
    }
    turtleV.clear();
    turtleV.resize(0);
}

void LSystem::generate(int count, glm::vec2 origin, glm::vec2 dir)
{
    destroy();
    Turtle* t = new Turtle(origin, dir, rand()%3+1, rand()%10+5);
    turtleV.push_back(t);
    for(int i = 0;i<count;i++)
    {
        parse();
    }
}

bool LSystem::get(glm::ivec2 pos)
{
    grid.getValue(pos);
}

void LSystem::parse()
{
    std::vector<Turtle*> tempTurtleV;
    for(int i = 0;i<turtleV.size();i++)
    {
        Turtle* curTurtle = turtleV[i];

        if(curTurtle->life<=0)//dead
        {

            int newNum = rand()%2+1;
            for(int n = 0;n<newNum;n++)
            {
                float degree = rand()%30 - 15;
                float speed = rand()%3+1;
                float life = rand()%10 + 5;
                glm::vec4 odir = glm::vec4(curTurtle->dir.x,0,curTurtle->dir.y,1);
                glm::vec4 ndir = glm::rotate(glm::mat4(),degree,glm::vec3(0,1,0))*odir;
                tempTurtleV.push_back(new Turtle(curTurtle->pos,
                                                 glm::vec2(ndir.x,ndir.z),
                                                 speed,
                                                 life));
            }
            delete curTurtle;
            turtleV[i] = nullptr;
        }
        else
        {
            grid.setLineValue(curTurtle->pos,curTurtle->pos+curTurtle->dir*curTurtle->speed,true,2);
            curTurtle->pos += curTurtle->dir*curTurtle->speed;
            curTurtle->life-=1;
            tempTurtleV.push_back(curTurtle);
        }
    }
    //printf("tempsize:%d---size:%d\n",tempTurtleV.size(),turtleV.size());
    turtleV = tempTurtleV;
}

void LSystem::printAll()
{
    grid.printAll();
}
