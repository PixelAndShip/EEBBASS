#include "plant.h"

Plant::Plant()
{
    health = 200;
    // energy = 100;
    x = 1000;
    y = 1000;
    plantColor.blue = 255;
    plantColor.transparency = 255;
    updateColor();
}

Plant::Plant(float eRadiation, std::mt19937 &gen)
{
    health = 200;
    // energy = 100;
    x = 1000;
    y = 1000;
    plantColor.blue = 255;
    plantColor.transparency = 255;
    std::uniform_int_distribution<> healthGen(-20, 50);
    health += healthGen(gen);
    updateColor();
}