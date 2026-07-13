#pragma once
#include "brain.h"
#include <random>

class Agent
{
private:
    float health;
    float energy;

    UnitColor agentColor;
    int x, y;

    float speed;
    Brain brain;

public:
    void updateColor()
    {
        DEBUG_LOG("Updating Agent color from stats");

        agentColor.red = std::clamp(static_cast<int>(std::round(energy)), 0, 255);
        agentColor.green = std::clamp(static_cast<int>(std::round(health)), 0, 255);
        agentColor.blue = std::clamp(static_cast<int>(std::round(speed)), 0, 255);

        DEBUG_LOG("Updated Agent color to: "
                  << agentColor.red << ", "
                  << agentColor.green << ", "
                  << agentColor.blue << ", "
                  << agentColor.transparency);
    }

    void setHealth(float iH)
    {
        DEBUG_LOG("Setting health from " << health << " to " << iH);
        health = iH;
    }

    void setEnergy(float iE)
    {
        DEBUG_LOG("Setting energy from " << energy << " to " << iE);
        energy = iE;
    }

    void setAgentColor(UnitColor iAC)
    {
        DEBUG_LOG("Setting Agent color from ("
                  << agentColor.red << ", "
                  << agentColor.green << ", "
                  << agentColor.blue << ", "
                  << agentColor.transparency << ") to ("
                  << iAC.red << ", "
                  << iAC.green << ", "
                  << iAC.blue << ", "
                  << iAC.transparency << ")");

        agentColor = iAC;
    }

    void setX(int iX)
    {
        DEBUG_LOG("Setting X from " << x << " to " << iX);
        x = iX;
    }

    void setY(int iY)
    {
        DEBUG_LOG("Setting Y from " << y << " to " << iY);
        y = iY;
    }

    void setCoords(std::string coords)
    {
        DEBUG_LOG("Setting coordinates from string: " << coords);

        auto _pos = coords.find("_");

        int aX, aY;

        try
        {
            aX = std::stoi(coords.substr(0, _pos));
            aY = std::stoi(coords.substr(_pos + 1));
        }
        catch (...)
        {
            DEBUG_LOG("Failed to parse coordinates");
            return;
        }

        x = aX;
        y = aY;

        DEBUG_LOG("Coordinates set to (" << x << ", " << y << ")");
    }

    void setSpeed(float iS)
    {
        DEBUG_LOG("Setting speed from " << speed << " to " << iS);
        speed = iS;
    }

    void setBrain() // idk if possible, brain consists of pointer tree
    {
        DEBUG_LOG("setBrain() called (currently unimplemented)");
    }

    float getHealth()
    {
        DEBUG_LOG("Getting health: " << health);
        return health;
    }

    float getEnergy()
    {
        DEBUG_LOG("Getting energy: " << energy);
        return energy;
    }

    UnitColor getAgentColor()
    {
        DEBUG_LOG("Getting Agent color: "
                  << agentColor.red << ", "
                  << agentColor.green << ", "
                  << agentColor.blue << ", "
                  << agentColor.transparency);

        return agentColor;
    }

    int getX()
    {
        DEBUG_LOG("Getting X: " << x);
        return x;
    }

    int getY()
    {
        DEBUG_LOG("Getting Y: " << y);
        return y;
    }

    float getSpeed()
    {
        DEBUG_LOG("Getting speed: " << speed);
        return speed;
    }

    const Brain &getBrain()
    {
        DEBUG_LOG("Getting Brain");
        return brain;
    }

    std::string getCoords()
    {
        std::string coords = std::to_string(x) + "_" + std::to_string(y);
        DEBUG_LOG("Getting coordinates: " << coords);
        return coords;
    }

    Agent();

    Agent(
        float eRadiation,
        std::mt19937 &gen,
        std::uniform_int_distribution<> &dist,
        int childNodeCount,
        int brainDepth);

    Agent(
        float iHealth,
        float iEnergy,
        float iSpeed,
        const Brain &iBrain,
        float eRadiation,
        std::mt19937 &gen,
        int childNodeCount,
        int brainDepth);

    void generateStart(std::mt19937 &gen);
};