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
    std::vector<unsigned int> generationID;
    unsigned int offspringCount;
    float speed;
    Brain brain;

public:
    void setProcessed(bool iP)
    {
        processed = iP;
    }
    void updateColor()
    {
        if (generationID.size() >= 1)
        {
            agentColor.red = std::clamp(static_cast<int>(getUsableColor(generationID[0])), 0, 255);
        }
        else
        {
            agentColor.red = 0;
        }
        agentColor.green = std::clamp(static_cast<int>(getUsableColor(generationID.size())), 0, 255);
        if (generationID.size() >= 1)
        {
            agentColor.blue = std::clamp(static_cast<int>(getUsableColor(generationID[generationID.size() - 1])), 0, 255);
        }
        else
        {
            agentColor.blue = 0;
        }

        agentColor.transparency = 255;
    }

    unsigned int getUsableColor(unsigned int iC)
    {
        if (iC > 255)
        {
            return getUsableColor((iC - 255));
        }
        return iC;
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
    void setGenerationID(std::vector<unsigned int> iGID)
    {
        generationID = iGID;
    }

    void appendToGenerationID(int iID)
    {
        generationID.push_back(iID);
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

    std::vector<unsigned int> getGenerationID()
    {
        return generationID;
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

    Agent(std::string selfData, std::string brainData, int iEID);

    Agent(
        int identifier,
        unsigned int iUniqueGenerationID,
        float eRadiation,
        int childNodeCount,
        int brainDepth);

    Agent(
        int identifier,
        std::vector<unsigned int> iGenerationID,
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

    void mutateStart();

    void logAgent(int iIteration, std::string fileName = "") const;

    std::string outputAgentStats() const;

    void constructCustomAgent(std::string data);
};