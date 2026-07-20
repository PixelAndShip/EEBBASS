#include "plant.h"

Plant::Plant()
{
    health = 100;
    // energy = 100;
    updateColor();
    x = 10;
    y = 10;
    plantColor.blue = 255;
}

Plant::Plant(float eRadiation, std::mt19937 &gen)
{
    health = 100;
    // energy = 100;
    x = 10;
    y = 10;
    plantColor.blue = 255;
    std::uniform_int_distribution<> healthGen(-50, 50);
    health += healthGen(gen);
    updateColor();
}