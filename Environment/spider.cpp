#include "spider.h"

bool Spider::seeSomething(char &unit)
{
    if (proximateAgents.empty() and unit == 'a')
    {
        return false;
    }
    else if (proximatePlants.empty() and unit == 'p')
    {
        return false;
    }
    return true;
}

bool Spider::seeColor(
    int red,
    int green,
    int blue,
    int transparency,
    int setRed,
    int setGreen,
    int setBlue,
    int setTransparency)
{
    int lRed = red - 10;
    int lGreen = green - 10;
    int lBlue = blue - 10;
    int hRed = red + 10;
    int hGreen = green + 10;
    int hBlue = blue + 10;
    bool redInRange = (setRed >= lRed and setRed <= hRed);
    bool greenInRange = (setGreen >= lGreen and setGreen <= hGreen);
    bool blueInRange = (setBlue >= lBlue and setBlue <= hBlue);
    return (redInRange and greenInRange and blueInRange);
}

bool Spider::energyCountAboveSet(float energy, float setAmount)
{
    return energy >= setAmount;
}

void Spider::updateSpeed(float deltaEnergy, Agent *Self)
{
    Self->energy -= deltaEnergy;
    Self->speed += deltaEnergy;

    Self->blue = std::round(Self->speed * 255);

    if (Self->speed < 0)
    {
        Self->speed = 0;
    }
    if (Self->energy < 0)
    {
        Self->energy = 0;
    }
}

void Spider::updateHealth(float deltaEnergy, Agent *Self)
{

    Self->energy -= deltaEnergy;
    Self->health += deltaEnergy;
    if (Self->health < 0)
    {
        Self->health = 0;
    }
    if (Self->energy < 0)
    {
        Self->energy = 0;
    }
    Self->green = std::round(Self->health * 255);
}

void Spider::bite(Agent *Opponent, float energyCost)
{
    Opponent->health -= energyCost;
}

void Spider::move(Agent *Self, char Direction)
{
    switch (Direction)
    {
    case 'u':
        Self->y -= 1;
    case 'd':
        Self->y += 1;
    case 'l':
        Self->x -= 1;
    case 'r':
        Self->x += 1;
    }
}
