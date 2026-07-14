#pragma once
#include "brain.h"
#include <random>

class Plant
{
private:
    float health;
    float energy;
    UnitColor plantColor;
    int x, y;

public:
    void updateColor()
    {
        DEBUG_LOG("Updating Plant color from stats");

        plantColor.red = std::clamp(static_cast<int>(std::round(energy)), 0, 255);
        plantColor.green = std::clamp(static_cast<int>(std::round(health)), 0, 255);

        DEBUG_LOG("Updated Plant color to: "
                  << plantColor.red << ", "
                  << plantColor.green << ", "
                  << plantColor.blue << ", "
                  << plantColor.transparency);
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

    void setPlantColor(UnitColor iAC)
    {
        DEBUG_LOG("Setting Agent color from ("
                  << plantColor.red << ", "
                  << plantColor.green << ", "
                  << plantColor.blue << ", "
                  << plantColor.transparency << ") to ("
                  << iAC.red << ", "
                  << iAC.green << ", "
                  << iAC.blue << ", "
                  << iAC.transparency << ")");

        plantColor = iAC;
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

    UnitColor getPlantColor()
    {
        DEBUG_LOG("Getting Agent color: "
                  << plantColor.red << ", "
                  << plantColor.green << ", "
                  << plantColor.blue << ", "
                  << plantColor.transparency);

        return plantColor;
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

    std::string getCoords()
    {
        std::string coords = std::to_string(x) + "_" + std::to_string(y);
        DEBUG_LOG("Getting coordinates: " << coords);
        return coords;
    }

    Plant();

    Plant(
        float eRadiation,
        std::mt19937 &gen);
};