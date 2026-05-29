#include "spider.h"

Spider::Spider()
{
    std::mt19937 g(rd());
    std::uniform_int_distribution<> d(0, 100);
    gen = g;
    dist = d;
}

bool Spider::seeSomething()
{
    if (!proximateCoords.empty())
    {
        return true;
    }
    return false;
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

bool Spider::ageCountAboveSet(float age, float setAmount)
{
    return age >= setAmount;
}

bool Spider::ageCountBelowSet(float age, float setAmount)
{
    return age < setAmount;
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
        break;
    case 'd':
        Self->setY(currentY + 1);
        break;
    case 'l':
        Self->setX(currentX - 1);
        break;
    case 'r':
        Self->setX(currentX + 1);
        break;
    }
}

std::vector<std::string> Spider::getProximateAgents(std::string coords)
{
    std::vector<std::string> proximateAgentCs = {};
    auto _pos = coords.find("_");
    int aX, aY;
    try
    {
        aX = std::stoi(coords.substr(0, _pos));
        aY = std::stoi(coords.substr(_pos + 1));
    }
    catch (...)
    {
        return proximateAgentCs;
    }

    std::vector<std::string> InRadius = {"0_-1", "1_-1", "1_0", "1_1", "0_1", "-1_1", "-1_0", "-1_-1"};
    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if (PastAgents.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) != PastAgents.end())
        {

            proximateAgentCs.push_back(std::to_string(aX + dx) + "_" + std::to_string(aY + dy));
        }
    }
    return proximateAgentCs;
}

std::vector<std::string> Spider::getProximatePlants(std::string coords)
{
    std::vector<std::string> proximatePlantCs = {};
    auto _pos = coords.find("_");
    int aX, aY;
    try
    {
        aX = std::stoi(coords.substr(0, _pos));
        aY = std::stoi(coords.substr(_pos + 1));
    }
    catch (...)
    {
        return proximatePlantCs;
    }

    std::vector<std::string> InRadius = {"0_-1", "1_-1", "1_0", "1_1", "0_1", "-1_1", "-1_0", "-1_-1"};
    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if (PastPlants.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) != PastPlants.end())
        {
            proximatePlantCs.push_back(std::to_string(aX + dx) + "_" + std::to_string(aY + dy));
        }
    }
    return proximatePlantCs;
}

void Spider::setProximities(std::string startCoords)
{

    std::queue<std::string> uncheckedQueue;
    proximateCoords[startCoords] = true;
    uncheckedQueue.push(startCoords);

    while (!uncheckedQueue.empty())
    {
        std::string current = uncheckedQueue.front();
        uncheckedQueue.pop();

        std::vector<std::string> proximateAgents = getProximateAgents(current);
        std::vector<std::string> proximatePlants = getProximatePlants(current);

        std::vector<std::string> coords;
        coords.reserve(proximateAgents.size() + proximatePlants.size());
        coords.insert(coords.end(), proximateAgents.begin(), proximateAgents.end());
        coords.insert(coords.end(), proximatePlants.begin(), proximatePlants.end());

        for (const std::string &coord : coords)
        {
            if (proximateCoords.find(coord) == proximateCoords.end())
            {
                proximateCoords[coord] = true;
                uncheckedQueue.push(coord);
            }
        }
    }
}
void Spider::manageSubMoment()
{

    if (proximateCoords.empty())
    {
        return;
    }

    for (auto cs : proximateCoords)
    {

        if (PastAgents.find(cs.first) == PastAgents.end())
        {
            continue;
        }

        OutputNode *action = nullptr;
        int actionKey = 255;

        if (PastAgents.at(cs.first) != nullptr and !PastAgents.at(cs.first)->getBrain().getInputNodes().empty())
        {
            action = getAction(PastAgents.at(cs.first)->getBrain().getInputNodes().at(0));
        }
        if (action != nullptr)
        {
            if (action->getKey() < getActions().size())
            {
                actionKey = action->getKey();
                std::cout << "\n"
                          << cs.first << ":Output: " << getActions().at(actionKey) << "\n";
                manageAction(PastAgents[cs.first], action);
            }
        }
    }
}

OutputNode *Spider::getAction(InputNode *parentNode)
{
    if (parentNode == nullptr or parentNode->getInputNodes().empty())
    {
        return nullptr;
    }
    float activateNode = dist(gen) / 100.0;

    for (InputNode *in : parentNode->getInputNodes())
    {
        if (in == nullptr or activateNode < in->getWeight())
        {
            continue;
        }

        if (in->getInputNodes().empty() and in->getOutputNode() != nullptr and in->getOutputNode()->getKey() != 255)
        {
            std::cout << getSenses().at(in->getKey()) << " ";
            return in->getOutputNode();
        }
        OutputNode *on = getAction(in);
        if (on != nullptr)
        {
            std::cout << getSenses().at(in->getKey()) << " ";
            return on;
        }
    }
    return nullptr;
}

void Spider::manageAction(Agent *Self, OutputNode *ActionNode)
{
    return;
}
