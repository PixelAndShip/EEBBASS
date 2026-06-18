#pragma once
#include "brain.h"
#include <random>
class Agent
{

private:
    float health;
    float energy;
    float age;

    UnitColor agentColor;
    int x, y;

    float speed;
    Brain brain;

public:
    void setHealth(float iH)
    {
        health = iH;
    }

    void setEnergy(float iE)
    {
        energy = iE;
    }
    void setAge(float iA)
    {
        age = iA;
    }
    void setAgentColor(UnitColor iAC)
    {
        agentColor = iAC;
    }

    void setX(int iX)
    {
        x = iX;
    }

    void setY(int iY)
    {
        y = iY;
    }

    void setCoords(std::string coords)
    {
        auto _pos = coords.find("_");
        int aX, aY;
        try
        {
            aX = std::stoi(coords.substr(0, _pos));
            aY = std::stoi(coords.substr(_pos + 1));
        }
        catch (...)
        {
            return;
        }
        x = aX;
        y = aY;
    }

    void setSpeed(float iS)
    {
        speed = iS;
    }

    void setBrain() // idk if possible, brain consists of pointer tree, issues might arise if copied over. Brain currently is created via constructor.
    {
    }

    float getHealth()
    {
        return health;
    }

    float getEnergy()
    {
        return energy;
    }
    float getAge()
    {
        return age;
    }
    UnitColor getAgentColor()
    {
        return agentColor;
    }

    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    float getSpeed()
    {
        return speed;
    }

    const Brain &getBrain()
    {
        return brain;
    }
    std::string getCoords()
    {
        return std::to_string(x) + "_" + std::to_string(y);
    }
    Agent();

    Agent(float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth); // default spawn

    Agent(float iHealth, float iEnergy, float iSpeed, float iSight, Brain &iBrain, float eRadiation); // split spawn

    void generateStart(std::mt19937 &gen);
};