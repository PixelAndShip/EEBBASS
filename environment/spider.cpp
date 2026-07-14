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

    for (auto pl1 : Plants)
    {
        DEBUG_LOG("Deleting Plant at coordinates: " << pl1.first
                                                    << " pointer: " << pl1.second);
        delete pl1.second;
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
            getProximateAgents(&Agents, current);

        std::vector<std::string> proximatePlants =
            getProximatePlants(&Plants, current);

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

        std::vector<InputNode *> rootNodes = agent->getBrain().getInputNodes();
        OutputNode *action = nullptr;
        for (InputNode *rootNode : rootNodes)
        {
            action = getAction(coords, rootNode);
            if (action != nullptr)
            {
                break;
            }
        }

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
        return seeSomething(&Agents, &Plants, AgentCoordinates);

    case 1:
        DEBUG_LOG("Sense: SeeColor");
        return seeColor(
            &Agents,
            &Plants,
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

    default:
        DEBUG_LOG("Unknown sense key");
        break;
    }

    return false;
}

OutputNode *Spider::getAction(
    std::string AgentCoordinates,
    InputNode *node)
{
    DEBUG_LOG("Getting action for Agent at "
              << AgentCoordinates);

    if (node == nullptr)
    {
        DEBUG_LOG("Node is nullptr");
        return nullptr;
    }

    float activateNode = dist(gen) / 100.0f;

    DEBUG_LOG("Activation roll: "
              << activateNode);

    DEBUG_LOG("Evaluating InputNode");
    DEBUG_LOG("Node key: "
              << node->getKey());

    float weight = node->getWeight();

    DEBUG_LOG("Node weight: "
              << weight);

    if (activateNode >= weight)
    {
        DEBUG_LOG("Node activation failed");
        return nullptr;
    }

    DEBUG_LOG("Node activated");

    if (!manageSense(AgentCoordinates, node))
    {
        DEBUG_LOG("Sense failed");
        return nullptr;
    }

    DEBUG_LOG("Sense passed");

    if (node->getOutputNode() != nullptr and
        node->getOutputNode()->getKey() != 255)
    {
        DEBUG_LOG("Checking OutputNode");

        DEBUG_LOG("Output weight: "
                  << node->getOutputNode()->getWeight());

        if (activateNode < node->getOutputNode()->getWeight())
        {
            DEBUG_LOG("OutputNode activated with key "
                      << node->getOutputNode()->getKey());

            return node->getOutputNode();
        }

        DEBUG_LOG("OutputNode activation failed");
    }

    DEBUG_LOG("Checking "
              << node->getInputNodes().size()
              << " child node(s)");

    for (InputNode *child : node->getInputNodes())
    {
        if (child == nullptr)
        {
            DEBUG_LOG("Skipping nullptr child");
            continue;
        }

        DEBUG_LOG("Descending into child");

        OutputNode *action =
            getAction(
                AgentCoordinates,
                child);

        if (action != nullptr)
        {
            DEBUG_LOG("Child returned action");
            return action;
        }

        DEBUG_LOG("Child returned no action");
    }

    DEBUG_LOG("No action found in this branch");

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
        move(&Agents, AgentCoordinates, 'l');
        break;

    case 1:
        DEBUG_LOG("Action: Move Right");
        move(&Agents, AgentCoordinates, 'r');
        break;

    case 2:
        DEBUG_LOG("Action: Move Up");
        move(&Agents, AgentCoordinates, 'u');
        break;

    case 3:
        DEBUG_LOG("Action: Move Down");
        move(&Agents, AgentCoordinates, 'd');
        break;

    case 4:
        DEBUG_LOG("Action: Bite");
        bite(
            &Agents,
            &Plants,
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
            &Agents,
            &Plants,
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
        getSplitCoords(&Agents, &Plants, ParentCoords);

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
    parent->setEnergy(parent->getEnergy() / 2.0f);

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