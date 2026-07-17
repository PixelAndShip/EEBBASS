#include "node_functions.h"

bool seeSomething(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates)
{
    DEBUG_LOG("Checking if Agent at " << AgentCoordinates << " sees something");

    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);
    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);

    if (proximateAgentCs.empty() and proximatePlantCs.empty())
    {
        DEBUG_LOG("Nothing nearby found");
        return false;
    }

    DEBUG_LOG("Found " << proximateAgentCs.size() << " nearby agent(s)");
    DEBUG_LOG("Found " << proximatePlantCs.size() << " nearby plant(s)");
    return true;
}

bool seeColor(
    const std::unordered_map<std::string, Agent *> *Agents,
    const std::unordered_map<std::string, Plant *> *Plants,
    std::string AgentCoordinates,
    UnitColor setC)
{
    DEBUG_LOG("Checking nearby colors around Agent at " << AgentCoordinates);

    DEBUG_LOG("Target color: "
              << setC.red << ", "
              << setC.green << ", "
              << setC.blue << ", "
              << setC.transparency);

    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);
    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);
    if (proximateAgentCs.empty() and proximatePlantCs.empty())
    {
        DEBUG_LOG("Nothing nearby found");
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
        DEBUG_LOG("Checking Agent at " << cs);

        ag = Agents->at(cs);

        if (ag != nullptr)
        {
            clr = ag->getAgentColor();

            r = clr.red;
            g = clr.green;
            b = clr.blue;
            t = clr.transparency;

            DEBUG_LOG("Agent color: "
                      << r << ", "
                      << g << ", "
                      << b << ", "
                      << t);

            r_in_range = r <= setC.red + 10 and r >= setC.red - 10;
            g_in_range = g <= setC.green + 10 and g >= setC.green - 10;
            b_in_range = b <= setC.blue + 10 and b >= setC.blue - 10;
            t_in_range = t <= setC.transparency + 10 and t >= setC.transparency - 10;

            if (r_in_range and g_in_range and b_in_range and t_in_range)
            {
                DEBUG_LOG("Matching color found");
                return true;
            }
        }
    }

    for (std::string cs : proximatePlantCs)
    {
        DEBUG_LOG("Checking Plant at " << cs);

        pl = Plants->at(cs);

        if (ag != nullptr)
        {
            clr = pl->getPlantColor();

            r = clr.red;
            g = clr.green;
            b = clr.blue;
            t = clr.transparency;

            DEBUG_LOG("Plant color: "
                      << r << ", "
                      << g << ", "
                      << b << ", "
                      << t);

            r_in_range = r <= setC.red + 10 and r >= setC.red - 10;
            g_in_range = g <= setC.green + 10 and g >= setC.green - 10;
            b_in_range = b <= setC.blue + 10 and b >= setC.blue - 10;
            t_in_range = t <= setC.transparency + 10 and t >= setC.transparency - 10;

            if (r_in_range and g_in_range and b_in_range and t_in_range)
            {
                DEBUG_LOG("Matching color found");
                return true;
            }
        }
    }

    DEBUG_LOG("No matching colors found");

    return false;
}

bool energyCountAboveSet(float energy, float setAmount)
{
    DEBUG_LOG("Checking energy above set amount: "
              << energy << " >= " << setAmount);

    return energy >= setAmount;
}

bool energyCountBelowSet(float energy, float setAmount)
{
    DEBUG_LOG("Checking energy below set amount: "
              << energy << " < " << setAmount);

    return energy < setAmount;
}

bool healthCountAboveSet(float health, float setAmount)
{
    DEBUG_LOG("Checking health above set amount: "
              << health << " >= " << setAmount);

    return health >= setAmount;
}

bool healthCountBelowSet(float health, float setAmount)
{
    DEBUG_LOG("Checking health below set amount: "
              << health << " < " << setAmount);

    return health < setAmount;
}

void updateSpeed(float deltaEnergy, Agent *Self)
{
    DEBUG_LOG("Updating Agent speed using energy: " << deltaEnergy);

    if (Self == nullptr)
    {
        DEBUG_LOG("Cannot update speed: Agent is nullptr");
        return;
    }

    float currentEnergy = Self->getEnergy();
    float currentSpeed = Self->getSpeed();

    DEBUG_LOG("Current energy: " << currentEnergy);
    DEBUG_LOG("Current speed: " << currentSpeed);

    if (currentEnergy <= 0.001)
    {
        DEBUG_LOG("Cannot increase speed: no energy");
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
        DEBUG_LOG("Speed below zero, setting to 0");
        Self->setSpeed(0);
    }

    if (Self->getEnergy() < 0)
    {
        DEBUG_LOG("Energy below zero, setting to 0");
        Self->setEnergy(0);
    }

    DEBUG_LOG("Finished updating speed");
}

void updateHealth(float deltaEnergy, Agent *Self)
{
    DEBUG_LOG("Updating Agent health using energy: " << deltaEnergy);

    if (Self == nullptr)
    {
        DEBUG_LOG("Cannot update health: Agent is nullptr");
        return;
    }

    float currentEnergy = Self->getEnergy();
    float currentHealth = Self->getHealth();

    DEBUG_LOG("Current energy: " << currentEnergy);
    DEBUG_LOG("Current health: " << currentHealth);

    if (currentEnergy <= 0.001 or currentHealth <= 0.001)
    {
        DEBUG_LOG("Cannot update health: insufficient energy or dead agent");
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
        DEBUG_LOG("Health below zero, setting to 0");
        Self->setHealth(0);
    }

    if (Self->getEnergy() < 0)
    {
        DEBUG_LOG("Energy below zero, setting to 0");
        Self->setEnergy(0);
    }

    DEBUG_LOG("Finished updating health");
}

void bite(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, float energyCost)
{
    DEBUG_LOG("Agent at " << AgentCoordinates
                          << " attempting bite with cost "
                          << energyCost);

    Agent *agent = Agents->at(AgentCoordinates);
    if (agent == nullptr)
    {
        DEBUG_LOG("Bite failed: Agent is nullptr");
        return;
    }
    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);

    if (!proximateAgentCs.empty())
    {
        Agent *ag = Agents->at(proximateAgentCs[0]);

        if (ag != nullptr)
        {
            DEBUG_LOG("Biting Agent at " << proximateAgentCs[0]);
            ag->setHealth(ag->getHealth() - energyCost);
            agent->setEnergy(agent->getEnergy() + energyCost * (1.0 - agent->getPlantDiet()));
            return;
        }
    }
    else
    {
        DEBUG_LOG("No nearby agents to bite");
    }

    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);

    if (!proximatePlantCs.empty())
    {
        Plant *pl = Plants->at(proximatePlantCs[0]);

        if (pl != nullptr)
        {
            DEBUG_LOG("Biting Plant at " << proximatePlantCs[0]);
            pl->setHealth(pl->getHealth() - energyCost);
            agent->setEnergy(agent->getEnergy() + energyCost * agent->getPlantDiet());
            return;
        }
    }
    else
    {
        DEBUG_LOG("No nearby plants to bite");
    }
    DEBUG_LOG("Finished bite");
}

void move(const std::unordered_map<std::string, Agent *> *Agents, std::string AgentCoordinates, char Direction)
{
    DEBUG_LOG("Moving Agent at "
              << AgentCoordinates
              << " direction "
              << Direction);

    int aX = Agents->at(AgentCoordinates)->getX();
    int aY = Agents->at(AgentCoordinates)->getY();

    switch (Direction)
    {
    case 'u':
        aY -= 20;
        break;

    case 'd':
        aY += 20;
        break;

    case 'l':
        aX -= 20;
        break;

    case 'r':
        aX += 20;
        break;
    }

    std::string coords =
        std::to_string(aX) + "_" + std::to_string(aY);

    DEBUG_LOG("New movement target: " << coords);
    Agents->at(AgentCoordinates)->setCoords(coords);
}

void biteColor(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, UnitColor Target, float energyCost)
{

    DEBUG_LOG("Agent at " << AgentCoordinates
                          << " attempting color bite");
    Agent *agent = Agents->at(AgentCoordinates);
    if (agent == nullptr)
    {
        DEBUG_LOG("Bite failed: Agent is nullptr");
        return;
    }
    DEBUG_LOG("Target color: "
              << Target.red << ", "
              << Target.green << ", "
              << Target.blue << ", "
              << Target.transparency);

    std::vector<std::string> proximateAgentCs = getProximateAgents(Agents, AgentCoordinates);
    std::vector<std::string> proximatePlantCs = getProximatePlants(Plants, AgentCoordinates);

    if (proximateAgentCs.empty())
    {
        DEBUG_LOG("No nearby agents to bite");
        return;
    }
    Agent *ag = nullptr;
    for (std::string cs : proximateAgentCs)
    {
        DEBUG_LOG("Checking Agent at " << cs);

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
                DEBUG_LOG("Matching color found, applying bite");

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
        DEBUG_LOG("Checking Plant at " << cs2);

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
                DEBUG_LOG("Matching color found, applying bite");

                pl->setHealth(
                    pl->getHealth() - energyCost);
                agent->setEnergy(agent->getEnergy() + energyCost * agent->getPlantDiet());
                return;
            }
        }
        pl = nullptr;
    }
    DEBUG_LOG("Finished color bite");
}

std::vector<std::string> getProximatePlants(const std::unordered_map<std::string, Plant *> *Plants, std::string coords)
{
    DEBUG_LOG("Getting proximate plants for coordinates: "
              << coords);

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
        DEBUG_LOG("Failed to parse coordinates");
        return proximatePlantCs;
    }

    std::vector<std::string> InRadius =
        {
            "0_-20",
            "20_-20",
            "20_0",
            "20_20",
            "0_20",
            "-20_20",
            "-20_0",
            "-20_-20"};

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
            DEBUG_LOG("Found proximate plant at "
                      << checkCoords);

            proximatePlantCs.push_back(checkCoords);
        }
    }

    DEBUG_LOG("Found "
              << proximatePlantCs.size()
              << " proximate plant(s)");

    return proximatePlantCs;
}

std::string getSplitCoords(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string ParentCoords)
{
    DEBUG_LOG("Finding split location for Agent at " << ParentCoords);

    auto _pos = ParentCoords.find("_");

    int aX, aY;

    try
    {
        aX = std::stoi(ParentCoords.substr(0, _pos));
        aY = std::stoi(ParentCoords.substr(_pos + 1));
    }
    catch (...)
    {
        DEBUG_LOG("Failed to parse parent coordinates");
        return "";
    }

    std::vector<std::string> InRadius =
        {
            "0_-20",
            "20_-20",
            "20_0",
            "20_20",
            "0_20",
            "-20_20",
            "-20_0",
            "-20_-20"};

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
            DEBUG_LOG("Found available split location: "
                      << newCoords);

            return newCoords;
        }
    }

    DEBUG_LOG("No available split location found");

    return "";
}

std::vector<std::string> getProximateAgents(const std::unordered_map<std::string, Agent *> *Agents, std::string coords)
{
    DEBUG_LOG("Getting proximate agents for coordinates: " << coords);

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
        DEBUG_LOG("Failed to parse coordinates: " << coords);
        return proximateAgentCs;
    }

    std::vector<std::string> InRadius =
        {
            "0_-20",
            "20_-20",
            "20_0",
            "20_20",
            "0_20",
            "-20_20",
            "-20_0",
            "-20_-20"};

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
            DEBUG_LOG("Found proximate Agent at " << checkCoords);
            proximateAgentCs.push_back(checkCoords);
        }
    }

    DEBUG_LOG("Found " << proximateAgentCs.size()
                       << " proximate agent(s)");

    return proximateAgentCs;
}