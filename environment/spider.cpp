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
    terrariumHeight = 800;
    terrariumWidth = 800;
}

Spider::~Spider()
{
    for (auto ag1 : Agents)
    {
        delete ag1.second;
    }
}
Spider::Spider(float rad, int maxBL, int maxBCN, int terW, int terH)
{
    std::mt19937 g(rd());
    std::uniform_int_distribution<> d(0, 100);
    gen = g;
    dist = d;
    radiation = rad;
    maxBrainChildNodes = maxBL;
    maxBrainLevel = maxBCN;
    terrariumHeight = terH;
    terrariumWidth = terW;
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
        ag = Agents.at(cs);
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
    if (currentEnergy <= 0)
    {
        return;
    }
    Self->setEnergy(currentEnergy - deltaEnergy);
    Self->setSpeed(currentSpeed + deltaEnergy);
    unsigned int blue = std::clamp(static_cast<int>(std::round(Self->getSpeed())), 0, 255);
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
    if (currentEnergy <= 0 or currentHealth <= 0)
    {
        return;
    }
    Self->setEnergy(currentEnergy - deltaEnergy);
    Self->setHealth(currentHealth + deltaEnergy);
    unsigned int green = std::clamp(static_cast<int>(std::round(Self->getHealth())), 0, 255);
    Self->setAgentColor({Self->getAgentColor().red, green, Self->getAgentColor().blue, Self->getAgentColor().transparency});

    if (Self->getHealth() < 0)
    {
        Self->setHealth(0);
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
    ag = Agents.at(proximateAgentCs[0]);
    if (ag != nullptr)
    {
        ag->setHealth(ag->getHealth() - energyCost);
    }
}

void Spider::move(std::string AgentCoordinates, char Direction) // move agent to newagents, check if can be moved to nextagents without collision
{
    int aX = Agents.at(AgentCoordinates)->getX();
    int aY = Agents.at(AgentCoordinates)->getY();
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

    std::string coords = std::to_string(aX) + "_" + std::to_string(aY);
    if (borderCheck(coords) == true)
    {
        Agents.at(AgentCoordinates)->setCoords(coords);
    }
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
        ag = Agents.at(cs);
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

    std::vector<std::string> InRadius = {"0_-20", "20_-20", "20_0", "20_20", "0_20", "-20_20", "-20_0", "-20_-20"};
    for (std::string r : InRadius)
    {
        int maxBrainLevel;
        int maxBrainChildNodes;
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if (Agents.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) != Agents.end())
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

    std::vector<std::string> InRadius = {"0_-20", "20_-20", "20_0", "20_10", "0_20", "-20_10", "-20_0", "-20_-20"};
    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if (Agents.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) == Agents.end() and Plants.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) == Plants.end())
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

    std::vector<std::string> InRadius = {"0_-20", "20_-20", "20_0", "20_10", "0_20", "-20_10", "-20_0", "-20_-20"};
    for (std::string r : InRadius)
    {
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if (Plants.find((std::to_string(aX + dx)) + "_" + std::to_string(aY + dy)) != Plants.end())
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

        if (Agents.find(cs.first) == Agents.end())
        {
            continue;
        }

        OutputNode *action = nullptr;
        int actionKey = 255;

        if (Agents.at(cs.first) != nullptr and !Agents.at(cs.first)->getBrain().getInputNodes().empty())
        {
            action = getAction(cs.first, Agents.at(cs.first)->getBrain().getInputNodes().at(0));
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

        // std::cout << "Adding " << cs.first
        //           << "  Next size = " << NextAgents.size()
        //           << '\n';
    }

    std::vector<std::string> sortedAgentsBySpeed = sortAgentsBySpeed(activeAgents); // incorporate manageAction for each

    for (std::string ag : sortedAgentsBySpeed)
    {
        manageAction(ag, activeAgentOutputs[ag]);
    }
    std::vector<std::pair<std::string, std::string>> moves;

    for (auto a : Agents)
    {
        if (a.first != a.second->getCoords())
            moves.emplace_back(a.first, a.second->getCoords());
    }

    for (auto [oldKey, newKey] : moves)
    {
        setNextAgent(oldKey, newKey, Agents.at(oldKey));
    }
}

std::vector<std::string> Spider::sortAgentsBySpeed(std::vector<std::string> agents)
{
    std::sort(agents.begin(), agents.end(),
              [this](const std::string &a, const std::string &b)
              {
                  auto agentA = Agents.at(a);
                  auto agentB = Agents.at(b);
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
        return energyCountAboveSet(Agents.at(AgentCoordinates)->getEnergy(), Sense->getSetAmount());
        break;
    case 3:
        return energyCountBelowSet(Agents.at(AgentCoordinates)->getEnergy(), Sense->getSetAmount());
        break;
    case 4:
        return healthCountAboveSet(Agents.at(AgentCoordinates)->getHealth(), Sense->getSetAmount());
        break;
    case 5:
        return healthCountBelowSet(Agents.at(AgentCoordinates)->getHealth(), Sense->getSetAmount());
        break;
    case 6:
        return ageCountAboveSet(Agents.at(AgentCoordinates)->getAge(), Sense->getSetAmount());
        break;
    case 7:
        return ageCountBelowSet(Agents.at(AgentCoordinates)->getAge(), Sense->getSetAmount());
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

        if (in->getOutputNode() != nullptr and in->getInputNodes().empty() and in->getOutputNode()->getKey() != 255 and activateNode >= in->getWeight())
        {
            // std::cout << getSenses().at(in->getKey()) << " ";
            if (activateNode >= in->getOutputNode()->getWeight())
            {
                return in->getOutputNode();
            }
        }

        OutputNode *on = getAction(AgentCoordinates, in);
        if (on != nullptr)
        {
            // std::cout << getSenses().at(in->getKey()) << " ";
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
    if (AgentCoordinates == "" or ActionNode == nullptr or Agents.at(AgentCoordinates)->getHealth() <= 0)
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
    case 2:
        move(AgentCoordinates, 'u');
        break;
    case 3:
        move(AgentCoordinates, 'd');
        break;
    case 4:
        bite(AgentCoordinates, ActionNode->getEnergyCost());
        break;
    case 5:
        splitNewAgent(AgentCoordinates);
        break;
    case 6:
        updateHealth(ActionNode->getEnergyCost(), Agents[AgentCoordinates]);
        break;
    case 7:
        updateSpeed(ActionNode->getEnergyCost(), Agents[AgentCoordinates]);
        break;
    case 8:
        biteColor(AgentCoordinates, ActionNode->getUnitColor(), ActionNode->getEnergyCost());
        break;
    default:
        break;
    }
}

bool Spider::borderCheck(std::string coords)
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
        return false;
    }
    bool inside = aX >= 0 and aX <= terrariumWidth and aY >= 0 and aY <= terrariumHeight;
    return inside;
}

void Spider::setNextAgent(std::string oldCoords, std::string newCoords, Agent *Self)
{
    bool spotNotTaken = Agents.find(newCoords) == Agents.end();
    bool notPlant = Plants.find(oldCoords) == Plants.end();
    bool insideBorders = borderCheck(newCoords) == true;
    // std::cout
    //     << "coords = " << coords
    //     << " transfer=" << spotNotTaken
    //     << " plant=" << notPlant
    //     << " border=" << insideBorders
    //     << '\n';

    if (spotNotTaken and notPlant and insideBorders)
    {
        if (Agents.find(oldCoords) != Agents.end())
        {
            Agents.erase(oldCoords);
        }
        Self->setCoords(newCoords);
        Agents[newCoords] = Self;
    }
    else
    {
        Self->setCoords(oldCoords);
    }
}

void Spider::splitNewAgent(std::string ParentCoords)
{
    if (Agents.find(ParentCoords) == Agents.end())
    {
        return;
    }
    std::string childCoords = getSplitCoords(ParentCoords);
    if (childCoords == "")
    {
        return;
    }

    Agent *childAgent = new Agent(Agents[ParentCoords]->getHealth(), Agents[ParentCoords]->getEnergy(), Agents[ParentCoords]->getSpeed(), Agents[ParentCoords]->getBrain(), radiation, gen, maxBrainChildNodes, maxBrainLevel); // implement split behavior
    setNextAgent(childCoords, childCoords, childAgent);
}
