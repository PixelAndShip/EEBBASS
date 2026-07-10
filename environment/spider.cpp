#include "spider.h"

Spider::Spider()
{
    DEBUG_LOG("Starting default Spider constructor");

    std::mt19937 g(rd());
    std::uniform_int_distribution<> d(0, 100);

    gen = g;
    dist = d;

    radiation = 0.5;
    maxBrainChildNodes = 3;
    maxBrainLevel = 4;

    terrariumHeight = 800;
    terrariumWidth = 800;

    DEBUG_LOG("Spider initialized with radiation: " << radiation);
    DEBUG_LOG("Max brain child nodes: " << maxBrainChildNodes);
    DEBUG_LOG("Max brain level: " << maxBrainLevel);
    DEBUG_LOG("Terrarium size: " << terrariumWidth << "x" << terrariumHeight);

    DEBUG_LOG("Finished default Spider constructor");
}

Spider::~Spider()
{
    DEBUG_LOG("Starting Spider destructor");

    DEBUG_LOG("Deleting " << Agents.size() << " agents");

    for (auto ag1 : Agents)
    {
        DEBUG_LOG("Deleting Agent at coordinates: " << ag1.first
                                                    << " pointer: " << ag1.second);

        delete ag1.second;
    }

    DEBUG_LOG("Finished Spider destructor");
}

Spider::Spider(float rad, int maxBL, int maxBCN, int terW, int terH)
{
    DEBUG_LOG("Starting custom Spider constructor");

    std::mt19937 g(rd());
    std::uniform_int_distribution<> d(0, 100);

    gen = g;
    dist = d;

    radiation = rad;
    maxBrainChildNodes = maxBCN;
    maxBrainLevel = maxBL;

    terrariumHeight = terH;
    terrariumWidth = terW;

    DEBUG_LOG("Spider radiation set to: " << radiation);
    DEBUG_LOG("Max brain child nodes set to: " << maxBrainChildNodes);
    DEBUG_LOG("Max brain level set to: " << maxBrainLevel);
    DEBUG_LOG("Terrarium size set to: "
              << terrariumWidth << "x" << terrariumHeight);

    DEBUG_LOG("Finished custom Spider constructor");
}

bool Spider::seeSomething(std::string AgentCoordinates)
{
    DEBUG_LOG("Checking if Agent at " << AgentCoordinates << " sees something");

    std::vector<std::string> proximateAgentCs = getProximateAgents(AgentCoordinates);

    if (proximateAgentCs.empty())
    {
        DEBUG_LOG("No nearby agents found");
        return false;
    }

    DEBUG_LOG("Found " << proximateAgentCs.size() << " nearby agent(s)");

    return true;
}

bool Spider::seeColor(
    std::string AgentCoordinates,
    UnitColor setC)
{
    DEBUG_LOG("Checking nearby colors around Agent at " << AgentCoordinates);

    DEBUG_LOG("Target color: "
              << setC.red << ", "
              << setC.green << ", "
              << setC.blue << ", "
              << setC.transparency);

    std::vector<std::string> proximateAgentCs = getProximateAgents(AgentCoordinates);

    if (proximateAgentCs.empty())
    {
        DEBUG_LOG("No nearby agents found");
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
        DEBUG_LOG("Checking Agent at " << cs);

        ag = Agents.at(cs);

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

    DEBUG_LOG("No matching colors found");

    return false;
}

bool Spider::energyCountAboveSet(float energy, float setAmount)
{
    DEBUG_LOG("Checking energy above set amount: "
              << energy << " >= " << setAmount);

    return energy >= setAmount;
}

bool Spider::energyCountBelowSet(float energy, float setAmount)
{
    DEBUG_LOG("Checking energy below set amount: "
              << energy << " < " << setAmount);

    return energy < setAmount;
}

bool Spider::healthCountAboveSet(float health, float setAmount)
{
    DEBUG_LOG("Checking health above set amount: "
              << health << " >= " << setAmount);

    return health >= setAmount;
}

bool Spider::healthCountBelowSet(float health, float setAmount)
{
    DEBUG_LOG("Checking health below set amount: "
              << health << " < " << setAmount);

    return health < setAmount;
}

bool Spider::ageCountAboveSet(float age, float setAmount)
{
    DEBUG_LOG("Checking age above set amount: "
              << age << " >= " << setAmount);

    return age >= setAmount;
}

bool Spider::ageCountBelowSet(float age, float setAmount)
{
    DEBUG_LOG("Checking age below set amount: "
              << age << " < " << setAmount);

    return age < setAmount;
}

void Spider::updateSpeed(float deltaEnergy, Agent *Self)
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

void Spider::updateHealth(float deltaEnergy, Agent *Self)
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

void Spider::bite(std::string AgentCoordinates, float energyCost)
{
    DEBUG_LOG("Agent at " << AgentCoordinates
                          << " attempting bite with cost "
                          << energyCost);

    std::vector<std::string> proximateAgentCs = getProximateAgents(AgentCoordinates);

    if (proximateAgentCs.empty())
    {
        DEBUG_LOG("No nearby agents to bite");
        return;
    }

    Agent *ag = Agents.at(proximateAgentCs[0]);

    if (ag != nullptr)
    {
        DEBUG_LOG("Biting Agent at " << proximateAgentCs[0]);

        ag->setHealth(ag->getHealth() - energyCost);
    }

    DEBUG_LOG("Finished bite");
}

void Spider::move(std::string AgentCoordinates, char Direction)
{
    DEBUG_LOG("Moving Agent at "
              << AgentCoordinates
              << " direction "
              << Direction);

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

    std::string coords =
        std::to_string(aX) + "_" + std::to_string(aY);

    DEBUG_LOG("New movement target: " << coords);

    if (borderCheck(coords))
    {
        DEBUG_LOG("Movement allowed");
        Agents.at(AgentCoordinates)->setCoords(coords);
    }
    else
    {
        DEBUG_LOG("Movement blocked by border");
    }
}

void Spider::biteColor(std::string AgentCoordinates, UnitColor Target, float energyCost)
{
    DEBUG_LOG("Agent at " << AgentCoordinates
                          << " attempting color bite");

    DEBUG_LOG("Target color: "
              << Target.red << ", "
              << Target.green << ", "
              << Target.blue << ", "
              << Target.transparency);

    std::vector<std::string> proximateAgentCs =
        getProximateAgents(AgentCoordinates);

    if (proximateAgentCs.empty())
    {
        DEBUG_LOG("No nearby agents to bite");
        return;
    }

    for (std::string cs : proximateAgentCs)
    {
        DEBUG_LOG("Checking Agent at " << cs);

        Agent *ag = Agents.at(cs);

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

                break;
            }
        }
    }

    DEBUG_LOG("Finished color bite");
}
std::vector<std::string> Spider::getProximateAgents(std::string coords)
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

        if (Agents.find(checkCoords) != Agents.end())
        {
            DEBUG_LOG("Found proximate Agent at " << checkCoords);
            proximateAgentCs.push_back(checkCoords);
        }
    }

    DEBUG_LOG("Found " << proximateAgentCs.size()
                       << " proximate agent(s)");

    return proximateAgentCs;
}

std::string Spider::getSplitCoords(std::string ParentCoords)
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
            "20_10",
            "0_20",
            "-20_10",
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

        if (Agents.find(newCoords) == Agents.end() and
            Plants.find(newCoords) == Plants.end())
        {
            DEBUG_LOG("Found available split location: "
                      << newCoords);

            return newCoords;
        }
    }

    DEBUG_LOG("No available split location found");

    return "";
}

std::vector<std::string> Spider::getProximatePlants(std::string coords)
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
            "20_10",
            "0_20",
            "-20_10",
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

        if (Plants.find(checkCoords) != Plants.end())
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

void Spider::setProximities(std::string startCoords)
{
    DEBUG_LOG("Setting proximities starting from "
              << startCoords);

    std::queue<std::string> uncheckedQueue;

    proximateCoords[startCoords] = true;
    uncheckedQueue.push(startCoords);

    int checkedCount = 0;

    while (!uncheckedQueue.empty())
    {
        std::string current = uncheckedQueue.front();
        uncheckedQueue.pop();

        checkedCount++;

        DEBUG_LOG("Checking proximity node "
                  << current);

        std::vector<std::string> proximateAgents =
            getProximateAgents(current);

        std::vector<std::string> proximatePlants =
            getProximatePlants(current);

        std::vector<std::string> coords;

        coords.reserve(
            proximateAgents.size() +
            proximatePlants.size());

        coords.insert(
            coords.end(),
            proximateAgents.begin(),
            proximateAgents.end());

        coords.insert(
            coords.end(),
            proximatePlants.begin(),
            proximatePlants.end());

        for (const std::string &coord : coords)
        {
            if (proximateCoords.find(coord) == proximateCoords.end())
            {
                DEBUG_LOG("Adding connected coordinate "
                          << coord);

                proximateCoords[coord] = true;
                uncheckedQueue.push(coord);
            }
        }
    }

    DEBUG_LOG("Finished setting proximities. Checked "
              << checkedCount
              << " coordinates");
}

void Spider::manageSubMoment()
{
    DEBUG_LOG("Starting sub moment management");

    actionQueue.clear();

    DEBUG_LOG("Cleared action queue");

    for (auto [coords, agent] : Agents)
    {
        DEBUG_LOG("Checking Agent at " << coords);

        if (agent == nullptr)
        {
            DEBUG_LOG("Skipping null Agent");
            continue;
        }

        if (agent->getHealth() <= 0.001)
        {
            DEBUG_LOG("Skipping dead Agent");
            continue;
        }

        if (agent->getBrain().getInputNodes().empty())
        {
            DEBUG_LOG("Skipping Agent with empty brain");
            continue;
        }

        OutputNode *action =
            getAction(
                coords,
                agent->getBrain().getInputNodes().at(0));

        if (action != nullptr and
            action->getKey() < getActions().size())
        {
            DEBUG_LOG("Queued action "
                      << action->getKey()
                      << " for Agent "
                      << coords);

            actionQueue.push_back(
                {coords, action});
        }
    }

    DEBUG_LOG("Sorting "
              << actionQueue.size()
              << " actions by speed");

    std::sort(
        actionQueue.begin(),
        actionQueue.end(),
        [this](const PendingAction &a,
               const PendingAction &b)
        {
            return Agents.at(a.coords)->getSpeed() >
                   Agents.at(b.coords)->getSpeed();
        });

    for (auto pending : actionQueue)
    {
        DEBUG_LOG("Executing queued action for "
                  << pending.coords);

        if (Agents.find(pending.coords) == Agents.end())
        {
            DEBUG_LOG("Agent no longer exists");
            continue;
        }

        manageAction(
            pending.coords,
            pending.action);
    }

    std::vector<std::pair<std::string, std::string>> moves;

    for (auto [oldCoords, agent] : Agents)
    {
        if (oldCoords != agent->getCoords())
        {
            DEBUG_LOG("Detected movement from "
                      << oldCoords
                      << " to "
                      << agent->getCoords());

            moves.push_back(
                {oldCoords, agent->getCoords()});
        }
    }

    for (auto [oldCoords, newCoords] : moves)
    {
        setNextAgent(
            oldCoords,
            newCoords,
            Agents.at(oldCoords));
    }

    DEBUG_LOG("Processing "
              << pendingBirths.size()
              << " pending births");

    for (auto [coords, child] : pendingBirths)
    {
        if (Agents.find(coords) == Agents.end() and
            Plants.find(coords) == Plants.end())
        {
            DEBUG_LOG("Adding child Agent at "
                      << coords);

            Agents[coords] = child;
            child->setCoords(coords);
        }
        else
        {
            DEBUG_LOG("Birth location occupied, deleting child");

            delete child;
        }
    }

    pendingBirths.clear();

    DEBUG_LOG("Finished sub moment management");
}
std::vector<std::string> Spider::sortAgentsBySpeed(std::vector<std::string> agents)
{
    DEBUG_LOG("Sorting " << agents.size() << " agents by speed");

    std::sort(
        agents.begin(),
        agents.end(),
        [this](const std::string &a, const std::string &b)
        {
            float speedA = Agents.at(a)->getSpeed();
            float speedB = Agents.at(b)->getSpeed();

            DEBUG_LOG("Comparing Agent speeds: "
                      << a << " (" << speedA << ") vs "
                      << b << " (" << speedB << ")");

            return speedA > speedB;
        });

    DEBUG_LOG("Finished sorting agents by speed");

    return agents;
}

bool Spider::manageSense(std::string AgentCoordinates, InputNode *Sense)
{
    DEBUG_LOG("Managing sense for Agent at "
              << AgentCoordinates);

    if (AgentCoordinates == "" or Sense == nullptr)
    {
        DEBUG_LOG("Invalid sense request");
        return false;
    }

    int key = Sense->getKey();

    DEBUG_LOG("Sense key: " << key);

    switch (key)
    {
    case 0:
        DEBUG_LOG("Sense: SeeSomething");
        return seeSomething(AgentCoordinates);

    case 1:
        DEBUG_LOG("Sense: SeeColor");
        return seeColor(
            AgentCoordinates,
            Sense->getUnitColor());

    case 2:
        DEBUG_LOG("Sense: EnergyCountAboveSet");
        return energyCountAboveSet(
            Agents.at(AgentCoordinates)->getEnergy(),
            Sense->getSetAmount());

    case 3:
        DEBUG_LOG("Sense: EnergyCountBelowSet");
        return energyCountBelowSet(
            Agents.at(AgentCoordinates)->getEnergy(),
            Sense->getSetAmount());

    case 4:
        DEBUG_LOG("Sense: HealthCountAboveSet");
        return healthCountAboveSet(
            Agents.at(AgentCoordinates)->getHealth(),
            Sense->getSetAmount());

    case 5:
        DEBUG_LOG("Sense: HealthCountBelowSet");
        return healthCountBelowSet(
            Agents.at(AgentCoordinates)->getHealth(),
            Sense->getSetAmount());

    case 6:
        DEBUG_LOG("Sense: AgeCountAboveSet");
        return ageCountAboveSet(
            Agents.at(AgentCoordinates)->getAge(),
            Sense->getSetAmount());

    case 7:
        DEBUG_LOG("Sense: AgeCountBelowSet");
        return ageCountBelowSet(
            Agents.at(AgentCoordinates)->getAge(),
            Sense->getSetAmount());

    default:
        DEBUG_LOG("Unknown sense key");
        break;
    }

    return false;
}

OutputNode *Spider::getAction(
    std::string AgentCoordinates,
    InputNode *parentNode)
{
    DEBUG_LOG("Getting action for Agent at "
              << AgentCoordinates);

    if (parentNode == nullptr)
    {
        DEBUG_LOG("Parent node is null");
        return nullptr;
    }

    if (parentNode->getInputNodes().empty())
    {
        DEBUG_LOG("Parent node has no children");
        return nullptr;
    }

    float activateNode = dist(gen) / 100.0;

    DEBUG_LOG("Activation roll: "
              << activateNode);

    for (InputNode *in : parentNode->getInputNodes())
    {
        if (in == nullptr)
        {
            DEBUG_LOG("Skipping null InputNode");
            continue;
        }

        float weight = in->getWeight();

        DEBUG_LOG("Checking InputNode with weight "
                  << weight);

        if (activateNode < weight)
        {
            DEBUG_LOG("Activation failed");
            continue;
        }

        if (!manageSense(AgentCoordinates, in))
        {
            DEBUG_LOG("Sense failed");
            continue;
        }

        DEBUG_LOG("Sense passed");

        if (in->getOutputNode() != nullptr and
            in->getInputNodes().empty() and
            in->getOutputNode()->getKey() != 255)
        {
            DEBUG_LOG("Checking OutputNode");

            if (activateNode >= in->getOutputNode()->getWeight())
            {
                DEBUG_LOG("OutputNode activated with key "
                          << in->getOutputNode()->getKey());

                return in->getOutputNode();
            }

            DEBUG_LOG("OutputNode activation failed");
        }

        OutputNode *on =
            getAction(
                AgentCoordinates,
                in);

        if (on != nullptr)
        {
            DEBUG_LOG("Child node returned action");
            return on;
        }
    }

    DEBUG_LOG("No action found");

    return nullptr;
}

void Spider::manageAction(
    std::string AgentCoordinates,
    OutputNode *ActionNode)
{
    DEBUG_LOG("Managing action for Agent at "
              << AgentCoordinates);

    if (AgentCoordinates == "" or
        ActionNode == nullptr)
    {
        DEBUG_LOG("Invalid action request");
        return;
    }

    if (Agents.at(AgentCoordinates)->getHealth() <= 0.001)
    {
        DEBUG_LOG("Cannot act: Agent is dead");
        return;
    }

    int key = ActionNode->getKey();

    DEBUG_LOG("Action key: " << key);

    switch (key)
    {
    case 0:
        DEBUG_LOG("Action: Move Left");
        move(AgentCoordinates, 'l');
        break;

    case 1:
        DEBUG_LOG("Action: Move Right");
        move(AgentCoordinates, 'r');
        break;

    case 2:
        DEBUG_LOG("Action: Move Up");
        move(AgentCoordinates, 'u');
        break;

    case 3:
        DEBUG_LOG("Action: Move Down");
        move(AgentCoordinates, 'd');
        break;

    case 4:
        DEBUG_LOG("Action: Bite");
        bite(
            AgentCoordinates,
            ActionNode->getEnergyCost());
        break;

    case 5:
        DEBUG_LOG("Action: Split");
        splitNewAgent(AgentCoordinates);
        break;

    case 6:
        DEBUG_LOG("Action: Convert energy to health");

        updateHealth(
            ActionNode->getEnergyCost(),
            Agents[AgentCoordinates]);
        break;

    case 7:
        DEBUG_LOG("Action: Convert energy to speed");

        updateSpeed(
            ActionNode->getEnergyCost(),
            Agents[AgentCoordinates]);
        break;

    case 8:
        DEBUG_LOG("Action: Bite color");

        biteColor(
            AgentCoordinates,
            ActionNode->getUnitColor(),
            ActionNode->getEnergyCost());
        break;

    default:
        DEBUG_LOG("Unknown action key");
        break;
    }

    DEBUG_LOG("Finished managing action");
}

bool Spider::borderCheck(std::string coords)
{
    DEBUG_LOG("Checking border for coordinates "
              << coords);

    auto _pos = coords.find("_");

    int aX, aY;

    try
    {
        aX = std::stoi(coords.substr(0, _pos));
        aY = std::stoi(coords.substr(_pos + 1));
    }
    catch (...)
    {
        DEBUG_LOG("Failed parsing coordinates");
        return false;
    }

    bool inside =
        aX >= 0 and
        aX <= terrariumWidth and
        aY >= 0 and
        aY <= terrariumHeight;

    DEBUG_LOG("Border result: "
              << (inside ? "inside" : "outside"));

    return inside;
}

void Spider::setNextAgent(
    std::string oldCoords,
    std::string newCoords,
    Agent *Self)
{
    DEBUG_LOG("Moving Agent from "
              << oldCoords
              << " to "
              << newCoords);

    bool spotNotTaken =
        Agents.find(newCoords) == Agents.end();

    bool notPlant =
        Plants.find(oldCoords) == Plants.end();

    bool insideBorders =
        borderCheck(newCoords);

    DEBUG_LOG("Movement checks - "
              << "spot available: " << spotNotTaken
              << ", not plant: " << notPlant
              << ", inside border: " << insideBorders);

    if (spotNotTaken and
        notPlant and
        insideBorders)
    {
        DEBUG_LOG("Movement approved");

        if (Agents.find(oldCoords) != Agents.end())
        {
            Agents.erase(oldCoords);
        }

        Self->setCoords(newCoords);

        Agents[newCoords] = Self;
    }
    else
    {
        DEBUG_LOG("Movement rejected");

        Self->setCoords(oldCoords);
    }
}

void Spider::splitNewAgent(std::string ParentCoords)
{
    DEBUG_LOG("Attempting split at "
              << ParentCoords);

    if (Agents.find(ParentCoords) == Agents.end())
    {
        DEBUG_LOG("Parent Agent does not exist");
        return;
    }
    std::string childCoords =
        getSplitCoords(ParentCoords);

    if (childCoords == "")
    {
        DEBUG_LOG("No available split location");
        return;
    }
    Agent *parent = Agents[ParentCoords];

    DEBUG_LOG("Splitting Agent with health "
              << parent->getHealth()
              << " and energy "
              << parent->getEnergy());

    parent->setHealth(
        parent->getHealth() / 2.0f);

    parent->setEnergy(
        parent->getEnergy() / 2.0f);

    DEBUG_LOG("Creating child Agent at "
              << childCoords);

    Agent *child = new Agent(
        parent->getHealth(),
        parent->getEnergy(),
        parent->getSpeed(),
        parent->getBrain(),
        radiation,
        gen,
        maxBrainChildNodes,
        maxBrainLevel);

    pendingBirths.push_back(
        {childCoords, child});

    DEBUG_LOG("Child Agent queued for birth");
}