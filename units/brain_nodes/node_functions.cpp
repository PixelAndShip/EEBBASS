#include "node_functions.h"

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

    unsigned int r, g, b, t;

    bool r_in_range = false;
    bool g_in_range = false;
    bool b_in_range = false;
    bool t_in_range = false;

    for (std::string cs : proximateAgentCs)
    {

        ag = Agents->at(cs);

        if (ag != nullptr)
        {
            clr = ag->getAgentColor();

            r = clr.red;
            g = clr.green;
            b = clr.blue;
            t = clr.transparency;

            r_in_range = r <= setC.red + 10 and r >= setC.red - 10;
            g_in_range = g <= setC.green + 10 and g >= setC.green - 10;
            b_in_range = b <= setC.blue + 10 and b >= setC.blue - 10;
            t_in_range = t <= setC.transparency + 10 and t >= setC.transparency - 10;

            if (r_in_range and g_in_range and b_in_range and t_in_range)
            {

                return true;
            }
        }
    }

    for (std::string cs : proximatePlantCs)
    {

        pl = Plants->at(cs);

        if (ag != nullptr)
        {
            clr = pl->getPlantColor();

            r = clr.red;
            g = clr.green;
            b = clr.blue;
            t = clr.transparency;

            r_in_range = r <= setC.red + 10 and r >= setC.red - 10;
            g_in_range = g <= setC.green + 10 and g >= setC.green - 10;
            b_in_range = b <= setC.blue + 10 and b >= setC.blue - 10;
            t_in_range = t <= setC.transparency + 10 and t >= setC.transparency - 10;

            if (r_in_range and g_in_range and b_in_range and t_in_range)
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
    DEBUG_LOG("returning" + std::to_string(health < setAmount));
    return health < setAmount;
}

// {6, "SeeSomethingLeft"},
// {7, "SeeSomethingRight"},
// {8, "SeeSomethingUp"},
// {9, "SeeSomethingDown"},
// {10, "SeeColorLeft"},
// {11, "SeeColorRight"},
// {12, "SeeColorUp"},
// {13, "SeeColorDown"}

bool seeSomething(int id, const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates)
{
    // need to check both coords and color if needed
    switch (id)
    {
    case 6:

        break;
    case 7:

        break;
    case 8:

        break;
    case 9:

        break;
    case 10:

        break;
    case 11:

        break;
    case 12:

        break;
    case 13:

        break;

    default:
        break;
    };
}

void updateSpeed(float deltaEnergy, Agent *Self)
{

    if (Self == nullptr)
    {

        return;
    }

    float currentEnergy = Self->getEnergy();
    float currentSpeed = Self->getSpeed();

    if (currentEnergy <= 0.001)
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

    if (currentEnergy <= 0.001 or currentHealth <= 0.001)
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

            ag->setHealth(ag->getHealth() - energyCost);
            agent->setEnergy(agent->getEnergy() + energyCost * (1.0 - agent->getPlantDiet()));
            return;
        }
    }

    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);

    if (!proximatePlantCs.empty())
    {
        Plant *pl = Plants->at(proximatePlantCs[0]);

        if (pl != nullptr)
        {

            pl->setHealth(pl->getHealth() - energyCost);
            agent->setEnergy(agent->getEnergy() + energyCost * agent->getPlantDiet());
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
        aY -= 8;
        break;

    case 'd':
        aY += 8;
        break;

    case 'l':
        aX -= 8;
        break;

    case 'r':
        aX += 8;
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

            bool r_in_range =
                clr.red <= Target.red + 10 and
                clr.red >= Target.red - 10;

            bool g_in_range =
                clr.green <= Target.green + 10 and
                clr.green >= Target.green - 10;

            bool b_in_range =
                clr.blue <= Target.blue + 10 and
                clr.blue >= Target.blue - 10;

            bool t_in_range =
                clr.transparency <= Target.transparency + 10 and
                clr.transparency >= Target.transparency - 10;

            if (r_in_range and
                g_in_range and
                b_in_range and
                t_in_range)
            {

                ag->setHealth(
                    ag->getHealth() - energyCost);
                agent->setEnergy(agent->getEnergy() + energyCost * (1.0 - agent->getPlantDiet()));
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

            bool r_in_range =
                clr.red <= Target.red + 10 and
                clr.red >= Target.red - 10;

            bool g_in_range =
                clr.green <= Target.green + 10 and
                clr.green >= Target.green - 10;

            bool b_in_range =
                clr.blue <= Target.blue + 10 and
                clr.blue >= Target.blue - 10;

            bool t_in_range =
                clr.transparency <= Target.transparency + 10 and
                clr.transparency >= Target.transparency - 10;

            if (r_in_range and
                g_in_range and
                b_in_range and
                t_in_range)
            {

                pl->setHealth(
                    pl->getHealth() - energyCost);
                agent->setEnergy(agent->getEnergy() + energyCost * agent->getPlantDiet());
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

    std::vector<std::string> InRadius =
        {
            "0_-8",
            "8_-8",
            "8_0",
            "8_8",
            "0_8",
            "-8_8",
            "-8_0",
            "-8_-8"};

    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");

        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));

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

    std::vector<std::string> InRadius =
        {
            "0_-8",
            "8_-8",
            "8_0",
            "8_8",
            "0_8",
            "-8_8",
            "-8_0",
            "-8_-8"};

    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");

        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));

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

    std::vector<std::string> InRadius =
        {
            "0_-8",
            "8_-8",
            "8_0",
            "8_8",
            "0_8",
            "-8_8",
            "-8_0",
            "-8_-8"};

    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");

        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));

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