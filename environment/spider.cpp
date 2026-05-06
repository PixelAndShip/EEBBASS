#include "spider.h"

bool Spider::seeSomething(char &unit)
{
    if (proximateAgentCoords.empty() and unit == 'a')
    {
        return false;
    }
    else if (proximatePlantCoords.empty() and unit == 'p')
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

bool Spider::energyCountBelowSet(float energy, float setAmount)
{
    return energy < setAmount;
}

bool Spider::healthCountAboveSet(float health, float setAmount)
{
    return health >= setAmount;
}

bool Spider::healthCountBelowSet(float health, float setAmount)
{
    return health < setAmount;
}

void Spider::updateSpeed(float deltaEnergy, Agent *Self)
{
    float currentEnergy = Self->getEnergy();
    float currentSpeed = Self->getSpeed();
    Self->setEnergy(currentEnergy - deltaEnergy);
    Self->setSpeed(currentSpeed += deltaEnergy);
    int blue = std::round(Self->getSpeed() * 255);
    Self->setAgentColor({Self->getAgentColor().red, Self->getAgentColor().green, blue, Self->getAgentColor().transparency});

    if (Self->getSpeed() < 0)
    {
        Self->setSpeed(0);
    }
    if (Self->getEnergy() < 0)
    {
        Self->setEnergy(0);
    }
}

void Spider::updateHealth(float deltaEnergy, Agent *Self)
{
    float currentEnergy = Self->getEnergy();
    float currentHealth = Self->getHealth();
    Self->setEnergy(currentEnergy - deltaEnergy);
    Self->setHealth(currentHealth += deltaEnergy);
    int green = std::round(Self->getHealth() * 255);
    Self->setAgentColor({Self->getAgentColor().red, green, Self->getAgentColor().blue, Self->getAgentColor().transparency});

    if (Self->getSpeed() < 0)
    {
        Self->setSpeed(0);
    }
    if (Self->getEnergy() < 0)
    {
        Self->setEnergy(0);
    }
}

void Spider::bite(auto *Opponent, float energyCost)
{

    if (typeid(*Opponent) == typeid(Agent))
    {
        float currentOpponentHealth = Opponent->getHealth();
        Opponent->setHealth(currentOpponentHealth -= energyCost);
    }
    else if (typeid(*Opponent) == typeid(Plant))
    {
        return;
    }
}

void Spider::move(Agent *Self, char Direction)
{
    int currentX = Self->getX();
    int currentY = Self->getY();
    switch (Direction)
    {
    case 'u':
        Self->setY(currentY - 1);
    case 'd':
        Self->setY(currentY + 1);
    case 'l':
        Self->setX(currentX - 1);
    case 'r':
        Self->setX(currentX + 1);
    }
}

void Spider::setProximities(std::string coords)
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
    std::vector<std::string> InRadius = {"0_-1", "1_-1", "1_0", "1_1", "0_1", "-1_1", "-1_0", "-1_-1"};
    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if (Agents.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) != Agents.end())
        {
            proximateAgentCoords.push_back(std::to_string(aX + dx) + "_" + std::to_string(aY + dy));
        } // maybe else if
        if (Plants.find(std::to_string(aX + dx) + "_" + std::to_string(aY + dy)) != Plants.end())
        {
            proximatePlantCoords.push_back(std::to_string(aX + dx) + "_" + std::to_string(aY + dy));
        }
    }
}

void Spider::manageSubMoment()
{
    if (proximateAgentCoords.empty())
    {
        return;
    }
    if (proximatePlantCoords.empty())
    {
        return;
    }
}

OutputNode Spider::getAction(InputNode *parentNode)
{
}
