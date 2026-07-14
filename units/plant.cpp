#include "plant.h"

Plant::Plant()
{
    health = 100;
    energy = 100;
    updateColor();
    x = 10;
    y = 10;
    plantColor.blue = 255;
}

Plant::Plant(float eRadiation, std::mt19937 &gen)
{
}