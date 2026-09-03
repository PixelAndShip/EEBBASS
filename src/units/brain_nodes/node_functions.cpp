#include "node_functions.h"

inline bool colorMatches(const UnitColor &a, const UnitColor &b, int tolerance = 25)
{
    return std::abs(static_cast<int>(a.red) - static_cast<int>(b.red)) <= tolerance and
           std::abs(static_cast<int>(a.green) - static_cast<int>(b.green)) <= tolerance and
           std::abs(static_cast<int>(a.blue) - static_cast<int>(b.blue)) <= tolerance and
           std::abs(static_cast<int>(a.transparency) - static_cast<int>(b.transparency)) <= tolerance;
}

bool touchSomething(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates)
{

    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);
    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);

    if (proximateAgentCs.empty() and proximatePlantCs.empty())
    {

        return false;
    }

    return true;
}

bool touchColor(
    const std::unordered_map<std::string, Agent *> *Agents,
    const std::unordered_map<std::string, Plant *> *Plants,
    std::string AgentCoordinates,
    UnitColor setC)
{

    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);
    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);
    if (proximateAgentCs.empty() and proximatePlantCs.empty())
    {

        return false;
    }

    Agent *ag = nullptr;
    Plant *pl = nullptr;
    UnitColor clr = {};

    for (std::string cs : proximateAgentCs)
    {

        ag = Agents->at(cs);

        if (ag != nullptr)
        {
            clr = ag->getAgentColor();

            if (colorMatches(clr, setC))
            {

                return true;
            }
        }
    }

    for (std::string cs : proximatePlantCs)
    {

        pl = Plants->at(cs);

        if (pl != nullptr)
        {
            clr = pl->getPlantColor();

            if (colorMatches(clr, setC))
            {

                return true;
            }
        }
    }

    return false;
}

bool energyCountAboveSet(float energy, float setAmount)
{

    return energy >= setAmount;
}

bool energyCountBelowSet(float energy, float setAmount)
{

    return energy < setAmount;
}

bool healthCountAboveSet(float health, float setAmount)
{

    return health >= setAmount;
}

bool healthCountBelowSet(float health, float setAmount)
{
    return health < setAmount;
}

bool seeSomething(int id, const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates)
{
    std::string coords = "";
    auto _pos = AgentCoordinates.find("_");

    int aX, aY;

    try
    {
        aX = std::stoi(AgentCoordinates.substr(0, _pos));
        aY = std::stoi(AgentCoordinates.substr(_pos + 1));
    }
    catch (...)
    {
        return false;
    }
    // checks sides via id (see in data_types.h get senses)
    switch (id)
    {
    case 6:

        for (int y : getDCoords())
        {
            coords = std::to_string(-2 + aX) + "_" + std::to_string(y + aY);
            if (Agents->find(coords) != Agents->end() or Plants->find(coords) != Plants->end())
            {
                return true;
            }
        }
        break;
    case 7:
        for (int y : getDCoords())
        {
            coords = std::to_string(1 + aX) + "_" + std::to_string(y + aY);
            if (Agents->find(coords) != Agents->end() or Plants->find(coords) != Plants->end())
            {
                return true;
            }
        }
        break;
    case 8:
        for (int x : getDCoords())
        {
            coords = std::to_string(aX + x) + "_" + std::to_string(-2 + aY);
            if (Agents->find(coords) != Agents->end() or Plants->find(coords) != Plants->end())
            {
                return true;
            }
        }
        break;
    case 9:
        for (int x : getDCoords())
        {
            coords = std::to_string(aX + x) + "_" + std::to_string(2 + aY);
            if (Agents->find(coords) != Agents->end() or Plants->find(coords) != Plants->end())
            {
                return true;
            }
        }
        break;
    default:
        break;
    };
    return false;
}

bool seeColor(int id, const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, UnitColor Target)
{

    std::string coords = "";
    auto _pos = AgentCoordinates.find("_");

    int aX, aY;

    try
    {
        aX = std::stoi(AgentCoordinates.substr(0, _pos));
        aY = std::stoi(AgentCoordinates.substr(_pos + 1));
    }
    catch (...)
    {
        return false;
    }
    UnitColor clr = {};
    switch (id)
    {
    case 10:

        for (int y : getDCoords())
        {
            coords = std::to_string(-2 + aX) + "_" + std::to_string(y + aY);
            if (Agents->find(coords) != Agents->end())
            {
                clr = Agents->at(coords)->getAgentColor();
            }
            else if (Plants->find(coords) != Plants->end())
            {
                clr = Plants->at(coords)->getPlantColor();
            }
            else
            {
                continue;
            }

            if (colorMatches(clr, Target))
            {
                return true;
            }
        }
        break;
    case 11:
        for (int y : getDCoords())
        {
            coords = std::to_string(2 + aX) + "_" + std::to_string(y + aY);
            if (Agents->find(coords) != Agents->end())
            {
                clr = Agents->at(coords)->getAgentColor();
            }
            else if (Plants->find(coords) != Plants->end())
            {
                clr = Plants->at(coords)->getPlantColor();
            }
            else
            {
                continue;
            }
            if (colorMatches(clr, Target))
            {
                return true;
            }
        }
        break;
    case 12:
        for (int x : getDCoords())
        {
            coords = std::to_string(aX + x) + "_" + std::to_string(-2 + aY);
            if (Agents->find(coords) != Agents->end())
            {
                clr = Agents->at(coords)->getAgentColor();
            }
            else if (Plants->find(coords) != Plants->end())
            {
                clr = Plants->at(coords)->getPlantColor();
            }
            else
            {
                continue;
            }
            if (colorMatches(clr, Target))
            {
                return true;
            }
        }
        break;
    case 13:
        for (int x : getDCoords())
        {
            coords = std::to_string(aX + x) + "_" + std::to_string(2 + aY);
            if (Agents->find(coords) != Agents->end())
            {
                clr = Agents->at(coords)->getAgentColor();
            }
            else if (Plants->find(coords) != Plants->end())
            {
                clr = Plants->at(coords)->getPlantColor();
            }
            else
            {
                continue;
            }
            if (colorMatches(clr, Target))
            {
                return true;
            }
        }
        break;
    default:
        break;
    };
    return false;
}

void updateSpeed(float deltaEnergy, Agent *Self)
{

    if (Self == nullptr)
    {

        return;
    }

    float currentEnergy = Self->getEnergy();
    float currentSpeed = Self->getSpeed();

    if (currentEnergy <= 0.001 or deltaEnergy > currentEnergy)
    {

        return;
    }

    Self->setEnergy(currentEnergy - deltaEnergy);
    Self->setSpeed(currentSpeed + deltaEnergy);

    unsigned int blue = std::clamp(
        static_cast<int>(std::round(Self->getSpeed())),
        0,
        255);

    UnitColor clr = Self->getAgentColor();

    Self->setAgentColor(
        {clr.red, clr.green, blue, clr.transparency});

    if (Self->getSpeed() < 0)
    {

        Self->setSpeed(0);
    }

    if (Self->getEnergy() < 0)
    {

        Self->setEnergy(0);
    }
}

void updateHealth(float deltaEnergy, Agent *Self)
{

    if (Self == nullptr)
    {

        return;
    }

    float currentEnergy = Self->getEnergy();
    float currentHealth = Self->getHealth();

    if (currentEnergy <= 0.001 or currentHealth <= 0.001 or deltaEnergy > currentEnergy)
    {

        return;
    }

    Self->setEnergy(currentEnergy - deltaEnergy);
    Self->setHealth(currentHealth + deltaEnergy);

    unsigned int green = std::clamp(
        static_cast<int>(std::round(Self->getHealth())),
        0,
        255);

    UnitColor clr = Self->getAgentColor();

    Self->setAgentColor(
        {clr.red, green, clr.blue, clr.transparency});

    if (Self->getHealth() < 0)
    {

        Self->setHealth(0);
    }

    if (Self->getEnergy() < 0)
    {

        Self->setEnergy(0);
    }
}

void bite(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, float energyCost)
{

    Agent *agent = Agents->at(AgentCoordinates);

    if (agent == nullptr)
    {
        return;
    }
    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);

    if (!proximateAgentCs.empty())
    {
        Agent *ag = Agents->at(proximateAgentCs[0]);

        if (ag != nullptr)
        {
            if (ag->getHealth() - energyCost <= 0)
            {
                ag->setHealth(0);
            }
            else
            {
                ag->setHealth(ag->getHealth() - energyCost);
            }

            agent->setEnergy(agent->getEnergy() + energyCost * (1.0 - agent->getPlantDiet()) - 0.5 * energyCost);

            return;
        }
    }

    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);

    if (!proximatePlantCs.empty())
    {
        Plant *pl = Plants->at(proximatePlantCs[0]);

        if (pl != nullptr)
        {

            if (pl->getHealth() - energyCost <= 0)
            {
                pl->setHealth(0);
            }
            else
            {
                pl->setHealth(pl->getHealth() - energyCost);
            }

            agent->setEnergy(agent->getEnergy() + energyCost * agent->getPlantDiet() - energyCost * 0.5);

            return;
        }
    }
}

void move(const std::unordered_map<std::string, Agent *> *Agents, std::string AgentCoordinates, char Direction)
{

    int aX = Agents->at(AgentCoordinates)->getX();
    int aY = Agents->at(AgentCoordinates)->getY();

    switch (Direction)
    {
    case 'u':
        aY -= 1;
        break;

    case 'd':
        aY += 1;
        break;

    case 'l':
        aX -= 1;
        break;

    case 'r':
        aX += 1;
        break;
    }

    std::string coords =
        std::to_string(aX) + "_" + std::to_string(aY);

    Agents->at(AgentCoordinates)->setCoords(coords);
}

void biteColor(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, UnitColor Target, float energyCost)
{

    Agent *agent = Agents->at(AgentCoordinates);

    if (agent == nullptr)
    {

        return;
    }

    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);
    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);

    if (proximateAgentCs.empty())
    {

        return;
    }
    Agent *ag = nullptr;
    for (std::string cs : proximateAgentCs)
    {

        ag = Agents->at(cs);

        if (ag != nullptr)
        {
            UnitColor clr = ag->getAgentColor();

            if (colorMatches(clr, Target))
            {

                if (ag->getHealth() - energyCost <= 0)
                {
                    ag->setHealth(0);
                }
                else
                {
                    ag->setHealth(ag->getHealth() - energyCost);
                }

                agent->setEnergy(agent->getEnergy() + energyCost * (1.0 - agent->getPlantDiet()) - energyCost * 0.5);

                return;
            }
        }
        ag = nullptr;
    }
    Plant *pl = nullptr;
    for (std::string cs2 : proximatePlantCs)
    {

        pl = Plants->at(cs2);

        if (pl != nullptr)
        {
            UnitColor clr = pl->getPlantColor();

            if (colorMatches(clr, Target))
            {

                if (pl->getHealth() - energyCost <= 0)
                {
                    pl->setHealth(0);
                }
                else
                {
                    pl->setHealth(pl->getHealth() - energyCost);
                }

                agent->setEnergy(agent->getEnergy() + energyCost * agent->getPlantDiet() - energyCost * 0.5);

                return;
            }
        }
        pl = nullptr;
    }
}

std::vector<std::string> getProximatePlants(const std::unordered_map<std::string, Plant *> *Plants, std::string coords)
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

    std::vector<std::vector<int>> InRadius =
        {
            {0, -1},
            {1, -1},
            {1, 0},
            {1, 1},
            {0, 1},
            {-1, 1},
            {-1, 0},
            {-1, -1}};

    for (auto pair : InRadius)
    {
        int dx = pair[0];
        int dy = pair[1];
        std::string checkCoords =
            std::to_string(aX + dx) + "_" +
            std::to_string(aY + dy);

        if (Plants->find(checkCoords) != Plants->end())
        {

            proximatePlantCs.push_back(checkCoords);
        }
    }

    return proximatePlantCs;
}

std::string getSplitCoords(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string ParentCoords)
{

    auto _pos = ParentCoords.find("_");

    int aX, aY;

    try
    {
        aX = std::stoi(ParentCoords.substr(0, _pos));
        aY = std::stoi(ParentCoords.substr(_pos + 1));
    }
    catch (...)
    {

        return "";
    }

    std::vector<std::vector<int>> InRadius =
        {
            {0, -1},
            {1, -1},
            {1, 0},
            {1, 1},
            {0, 1},
            {-1, 1},
            {-1, 0},
            {-1, -1}};

    for (auto pair : InRadius)
    {
        int dx = pair[0];
        int dy = pair[1];
        std::string newCoords =
            std::to_string(aX + dx) + "_" +
            std::to_string(aY + dy);

        if (Agents->find(newCoords) == Agents->end() and
            Plants->find(newCoords) == Plants->end())
        {

            return newCoords;
        }
    }

    return "";
}

std::vector<std::string> getProximateAgents(const std::unordered_map<std::string, Agent *> *Agents, std::string coords)
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

    std::vector<std::vector<int>> InRadius =
        {
            {0, -1},
            {1, -1},
            {1, 0},
            {1, 1},
            {0, 1},
            {-1, 1},
            {-1, 0},
            {-1, -1}};

    for (auto pair : InRadius)
    {
        int dx = pair[0];
        int dy = pair[1];

        std::string checkCoords =
            std::to_string(aX + dx) + "_" +
            std::to_string(aY + dy);

        if (Agents->find(checkCoords) != Agents->end())
        {

            proximateAgentCs.push_back(checkCoords);
        }
    }

    return proximateAgentCs;
}