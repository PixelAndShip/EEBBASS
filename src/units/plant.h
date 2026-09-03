
#pragma once
#include "brain.h"
#include <random>

class Plant
{
private:
    int env_identifier;
    float health;
    UnitColor plantColor;
    int x, y;

public:
    void setIdentifier(int iD) { env_identifier = iD; };
    int getIdentifier() { return env_identifier; };

    void updateColor()
    {
        plantColor.blue = std::clamp(static_cast<int>(std::round(health)), 0, 255);
    }

    void setHealth(float iH)
    {

        health = iH;
    }

    void setPlantColor(UnitColor iAC)
    {

        plantColor = iAC;
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

    float getHealth()
    {

        return health;
    }

    UnitColor getPlantColor()
    {

        return plantColor;
    }

    int getX()
    {

        return x;
    }

    int getY()
    {

        return y;
    }

    std::string getCoords()
    {
        std::string coords = std::to_string(x) + "_" + std::to_string(y);

        return coords;
    }

    Plant();

    Plant(
        float eRadiation,
        std::mt19937 &gen);
};