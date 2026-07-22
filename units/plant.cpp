#include "plant.h"

Plant::Plant()
{
    health = 200;
    // energy = 100;
    x = 10;
    y = 10;
    plantColor.blue = 255;
    updateColor();
}

Plant::Plant(float eRadiation, std::mt19937 &gen)
{
    health = 200;
    // energy = 100;
    x = 10;
    y = 10;
    plantColor.blue = 255;
    std::uniform_int_distribution<> healthGen(-50, 50);
    health += healthGen(gen);
    updateColor();
}