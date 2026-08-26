#pragma once

#include "brain.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <string>
class Agent
{
private:
    bool processed;
    int env_identifier;
    float health;
    float energy;
    float plantDiet;
    UnitColor agentColor;
    int x, y;

    float speed;
    Brain brain;

public:
    void setProcessed(bool iP)
    {
        processed = iP;
    }
    void updateColor()
    {

        agentColor.red = std::clamp(static_cast<int>(std::round(energy)), 0, 255);
        agentColor.green = std::clamp(static_cast<int>(std::round(health)), 0, 255);
        agentColor.blue = std::clamp(static_cast<int>(std::round(speed)), 0, 255);
    }

    void setIdentifier(int iD)
    {
        env_identifier = iD;
    }

    void setHealth(float iH)
    {

        health = iH;
    }

    void setEnergy(float iE)
    {

        energy = iE;
    }

    void setPlantDiet(float iPD)
    {

        plantDiet = iPD;
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

    void setBrain() // idk if possible, brain consists of pointer tree
    {
    }

    bool getProcessed()
    {
        return processed;
    }
    int getIdentifier()
    {
        return env_identifier;
    }

    float getHealth()
    {

        return health;
    }

    float getEnergy()
    {

        return energy;
    }

    float getPlantDiet()
    {

        return plantDiet;
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
        std::string coords = std::to_string(x) + "_" + std::to_string(y);

        return coords;
    }

    Agent();

    Agent(std::string selfData, std::string brainData, int iEID);

    Agent(std::string fileName, bool iP, int iEID, float iH, float iE, float iPD, UnitColor iC, int ix, int iy, float iS);

    Agent(
        int identifier,
        float eRadiation,
        int childNodeCount,
        int brainDepth);
    Agent(
        int identifier,
        float iHealth,
        float iEnergy,
        float iPlantDiet,
        float iSpeed,
        const Brain &iBrain,
        float eRadiation,
        int maxRootNodesCount,
        int childNodeCount,
        int brainDepth);

    void generateStart();

    void logAgent(std::string fileName = "") const;

    std::string outputAgentStats() const;

    void constructCustomAgent(std::string data);
};