#pragma once
#include "brain.h"
#include <random>
class Agent
{

private:
    float health;
    float bite;
    float energy;

    UnitColor agentColor;
    int x, y;

    float speed;
    Brain brain;

public:
    void setHealth(float iH)
    {
        health = iH;
    }

    void setBite(float iB)
    {
        bite = iB;
    }
    void setEnergy(float iE)
    {
        energy = iE;
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

    float getBite()
    {
        return bite;
    }

    float getEnergy()
    {
        return energy;
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

    Agent();

    Agent(float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist); // default spawn

    Agent(float iHealth, float iEnergy, float iSpeed, float iSight, Brain &iBrain, float eRadiation); // split spawn

    void generateStart(std::mt19937 &gen);
};