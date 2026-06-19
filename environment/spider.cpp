#include "spider.h"

Spider::Spider()
{
    std::mt19937 g(rd());
    std::uniform_int_distribution<> d(0, 100);
    gen = g;
    dist = d;
    radiation = 0.5;
    maxBrainChildNodes = 3;
    maxBrainLevel = 4;
}

bool Spider::seeSomething(std::string AgentCoordinates)
{
    std::vector<std::string> proximateAgentCs = getProximateAgents(AgentCoordinates);
    if (proximateAgentCs.empty())
    {
        return false;
    }
    return true;
}

bool Spider::seeColor(
    std::string AgentCoordinates,
    UnitColor setC)
{
    std::vector<std::string> proximateAgentCs = getProximateAgents(AgentCoordinates);
    if (proximateAgentCs.empty())
    {
        return false;
    }
    Agent *ag = nullptr;
    UnitColor clr = {};
    unsigned int r, g, b, t;
    bool r_in_range = false;
    bool g_in_range = false;
    bool b_in_range = false;
    bool t_in_range = false;
    for (std::string cs : proximateAgentCs)
    {
        ag = PastAgents.at(cs);
        if (ag != nullptr)
        {
            clr = ag->getAgentColor();
            r = clr.red;
            g = clr.green;
            b = clr.blue;
            t = clr.transparency;
            r_in_range = r <= setC.red + 10 and r >= setC.red - 10;
            g_in_range = g <= setC.green + 10 and r >= setC.green - 10;
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
    unsigned int blue = std::round(Self->getSpeed() * 255);
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
    unsigned int green = std::round(Self->getHealth() * 255);
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

void Spider::bite(std::string AgentCoordinates, float energyCost)
{
    std::vector<std::string> proximateAgentCs = getProximateAgents(AgentCoordinates);
    if (proximateAgentCs.empty())
    {
        return;
    }
    Agent *ag = nullptr;
    ag = PastAgents[proximateAgentCs[0]];
    if (ag != nullptr)
    {
        ag->setHealth(ag->getHealth() - energyCost);
    }
}

void Spider::move(std::string AgentCoordinates, char Direction) // move agent to newagents, check if can be moved to nextagents without collision
{
    int aX = PastAgents.at(AgentCoordinates)->getX();
    int aY = PastAgents.at(AgentCoordinates)->getY();
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
    std::string coords = aX + "_" + aY;
    setNextAgent(coords, PastAgents[AgentCoordinates]);
}

void Spider::biteColor(std::string AgentCoordinates, UnitColor Target, float energyCost)
{
    std::vector<std::string> proximateAgentCs = getProximateAgents(AgentCoordinates);
    if (proximateAgentCs.empty())
    {
        return;
    }
    Agent *ag = nullptr;
    UnitColor clr = {};
    unsigned int r, g, b, t;
    bool r_in_range = false;
    bool g_in_range = false;
    bool b_in_range = false;
    bool t_in_range = false;
    for (std::string cs : proximateAgentCs)
    {
        ag = PastAgents.at(cs);
        if (ag != nullptr)
        {
            clr = ag->getAgentColor();
            r = clr.red;
            g = clr.green;
            b = clr.blue;
            t = clr.transparency;
            r_in_range = r <= Target.red + 10 and r >= Target.red - 10;
            g_in_range = g <= Target.green + 10 and r >= Target.green - 10;
            b_in_range = b <= Target.blue + 10 and b >= Target.blue - 10;
            t_in_range = t <= Target.transparency + 10 and t >= Target.transparency - 10;
            if (r_in_range and g_in_range and b_in_range and t_in_range)
            {
                ag->setHealth(ag->getHealth() - energyCost);
                break;
            }
        }
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
        int maxBrainLevel;
        int maxBrainChildNodes;
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

std::string Spider::getSplitCoords(std::string ParentCoords)
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

    std::vector<std::string> InRadius = {"0_-1", "1_-1", "1_0", "1_1", "0_1", "-1_1", "-1_0", "-1_-1"};
    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if (PastAgents.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) == PastAgents.end() and PastPlants.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) == PastPlants.end())
        {
            return ((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy));
        }
    }
    return "";
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
    std::unordered_map<std::string, OutputNode *> activeAgentOutputs = {};
    std::vector<std::string> activeAgents = {};
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
            action = getAction(cs.first, PastAgents.at(cs.first)->getBrain().getInputNodes().at(0));
        }
        if (action != nullptr)
        {
            if (action->getKey() < getActions().size())
            {
                // actionKey = action->getKey();
                // std::cout << "\n" << cs.first << ":Output: " << getActions().at(actionKey) << "\n";
                activeAgentOutputs[cs.first] = action;
                activeAgents.push_back(cs.first);
            }
        }
    }

    std::vector<std::string> sortedAgentsBySpeed = sortAgentsBySpeed(activeAgents); // incorporate manageAction for each
}

std::vector<std::string> Spider::sortAgentsBySpeed(std::vector<std::string> agents)
{
    std::sort(agents.begin(), agents.end(),
              [this](const std::string &a, const std::string &b)
              {
                  auto agentA = PastAgents.at(a);
                  auto agentB = PastAgents.at(b);
                  return agentA->getSpeed() > agentB->getSpeed();
              });
    return agents;
}

// {0, "SeeSomething"},
// {1, "SeeColor"},
// {2, "EnergyCountAboveSet"},
// {3, "EnergyCountBelowSet"},
// {4, "HealthCountAboveSet"},
// {5, "HealthCountBelowSet"},
// {6, "AgeCountAboveSet"},
// {7, "AgeCountBelowSet"}};

bool Spider::manageSense(std::string AgentCoordinates, InputNode *Sense)
{
    if (AgentCoordinates == "" or Sense == nullptr)
    {
        return false;
    }
    int key = Sense->getKey();
    switch (key)
    {
    case 0:
        return seeSomething(AgentCoordinates);
        break;
    case 1:
        return seeColor(AgentCoordinates, Sense->getUnitColor());
        break;
    case 2:
        return energyCountAboveSet(PastAgents[AgentCoordinates]->getEnergy(), Sense->getSetAmount());
        break;
    case 3:
        return energyCountBelowSet(PastAgents[AgentCoordinates]->getEnergy(), Sense->getSetAmount());
        break;
    case 4:
        return healthCountAboveSet(PastAgents[AgentCoordinates]->getHealth(), Sense->getSetAmount());
        break;
    case 5:
        return healthCountBelowSet(PastAgents[AgentCoordinates]->getHealth(), Sense->getSetAmount());
        break;
    case 6:
        return ageCountAboveSet(PastAgents[AgentCoordinates]->getAge(), Sense->getSetAmount());
        break;
    case 7:
        return ageCountBelowSet(PastAgents[AgentCoordinates]->getAge(), Sense->getSetAmount());
        break;
    default:
        break;
    }
    return false;
}

OutputNode *Spider::getAction(std::string AgentCoordinates, InputNode *parentNode) // incorporate manageSense
{
    if (parentNode == nullptr or parentNode->getInputNodes().empty())
    {
        return nullptr;
    }
    float activateNode = dist(gen) / 100.0;

    for (InputNode *in : parentNode->getInputNodes())
    {
        if (in == nullptr or activateNode < in->getWeight() or manageSense(AgentCoordinates, in) == false)
        {
            continue;
        }

        if (in->getInputNodes().empty() and in->getOutputNode() != nullptr and in->getOutputNode()->getKey() != 255)
        {
            std::cout << getSenses().at(in->getKey()) << " ";
            return in->getOutputNode();
        }

        OutputNode *on = getAction(AgentCoordinates, in);
        if (on != nullptr)
        {
            std::cout << getSenses().at(in->getKey()) << " ";
            return on;
        }
    }
    return nullptr;
}

// {0, "MoveLeft"},
// {1, "MoveRight"},
// {2, "MoveUp"},
// {3, "MoveDown"},
// {4, "Bite"},
// {5, "Split"},
// {6, "ExpendEnergyOnHealth"}, // passive action
// {7, "ExpendEnergyOnSpeed"},  // passive action
// {8, "BiteColor"}};

void Spider::manageAction(std::string AgentCoordinates, OutputNode *ActionNode)
{
    if (AgentCoordinates == "" or ActionNode == nullptr)
    {
        return;
    }
    int key = ActionNode->getKey();
    switch (key)
    {
    case 0:
        move(AgentCoordinates, 'l');
        break;
    case 1:
        move(AgentCoordinates, 'r');
        break;
        int Radiation = 0.5;
    case 2:
        move(AgentCoordinates, 'u');
        break;
    case 3:
        move(AgentCoordinates, 'd');
        break;
    case 4:
        bite(AgentCoordinates, ActionNode->getEnergyCost());
    case 5:
        splitNewAgent(AgentCoordinates);
        break;
    case 6:
        updateHealth(ActionNode->getEnergyCost(), PastAgents[AgentCoordinates]);
        break;
    case 7:
        updateSpeed(ActionNode->getEnergyCost(), PastAgents[AgentCoordinates]);
        break;
    case 8:
        biteColor(AgentCoordinates, ActionNode->getUnitColor(), ActionNode->getEnergyCost());
        break;
    default:
        break;
    }
}

void Spider::setNextAgent(std::string coords, Agent *Self)
{
    // need to implement border check
    if (NextAgents.find(coords) == NextAgents.end() and NextPlants.find(coords) == NextPlants.end())
    {
        Self->setCoords(coords);
        NextAgents[coords] = Self;
    }
    else
    {
        delete Self;
    }
}

void Spider::splitNewAgent(std::string ParentCoords)
{
    if (PastAgents.find(ParentCoords) == PastAgents.end())
    {
        return;
    }
    std::string childCoords = getSplitCoords(ParentCoords);
    if (childCoords == "")
    {
        return;
    }

    Agent *childAgent = new Agent(PastAgents[ParentCoords]->getHealth(), PastAgents[ParentCoords]->getEnergy(), PastAgents[ParentCoords]->getSpeed(), PastAgents[ParentCoords]->getBrain(), radiation, gen, maxBrainChildNodes, maxBrainLevel); // implement split behavior
    setNextAgent(childCoords, childAgent);
}