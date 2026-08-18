#include "spider.h"

Spider::~Spider()
{

    for (auto ag1 : Agents)
    {

        delete ag1.second;
    }

    for (auto pl1 : Plants)
    {

        delete pl1.second;
    }
}

Spider::Spider(float rad, int maxBL, int maxBCN, int terW, int terH)
{

    std::mt19937 g(rd());
    std::uniform_int_distribution<> d(0, 100);

    gen = g;
    dist = d;

    radiation = rad;
    maxBrainChildNodes = maxBCN;
    maxBrainLevel = maxBL;

    terrariumHeight = terH;
    terrariumWidth = terW;
}

// void Spider::setProximities(std::string startCoords)
// {
//     DEBUG_LOG("Setting proximities starting from "
//               << startCoords);

//     std::queue<std::string> uncheckedQueue;

//     proximateCoords[startCoords] = true;
//     uncheckedQueue.push(startCoords);

//     int checkedCount = 0;

//     while (!uncheckedQueue.empty())
//     {
//         std::string current = uncheckedQueue.front();
//         uncheckedQueue.pop();

//         checkedCount++;

//         DEBUG_LOG("Checking proximity node "
//                   << current);

//         std::vector<std::string> proximateAgents =
//             getProximateAgents(&Agents, current);

//         std::vector<std::string> proximatePlants =
//             getProximatePlants(&Plants, current);

//         std::vector<std::string> coords;

//         coords.reserve(
//             proximateAgents.size() +
//             proximatePlants.size());

//         coords.insert(
//             coords.end(),
//             proximateAgents.begin(),
//             proximateAgents.end());

//         coords.insert(
//             coords.end(),
//             proximatePlants.begin(),
//             proximatePlants.end());

//         for (const std::string &coord : coords)
//         {
//             if (proximateCoords.find(coord) == proximateCoords.end())
//             {
//                 DEBUG_LOG("Adding connected coordinate "
//                           << coord);

//                 proximateCoords[coord] = true;
//                 uncheckedQueue.push(coord);
//             }
//         }
//     }

//     DEBUG_LOG("Finished setting proximities. Checked "
//               << checkedCount
//               << " coordinates");
// }

void Spider::manageSubMoment()
{

    actionQueue.clear();
    // processedAgents.clear();

    for (auto &[coords, bol] : Agents)
    {

        auto it = Agents.find(coords);

        if (it == Agents.end() or it->second == nullptr)
        {

            continue;
        }

        Agent *agent = it->second;

        if (agent->getHealth() <= 0.1)
        {

            continue;
        }

        if (agent->getBrain().getInputNodes().empty())
        {

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

            actionQueue.push_back(
                {coords, action});
        }
    }

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

        if (Agents.find(pending.coords) == Agents.end())
        {

            continue;
        }

        if (Agents[pending.coords]->getProcessed() == false)
        {
            Agents[pending.coords]->setProcessed(true);
            manageAction(
                pending.coords,
                pending.action);
        }
    }

    std::vector<std::pair<std::string, std::string>> moves;

    for (auto &[oldCoords, exists] : Agents)
    {
        auto it = Agents.find(oldCoords);

        if (it == Agents.end())
        {
            continue;
        }

        Agent *agent = it->second;

        if (oldCoords != agent->getCoords())
        {
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

    for (auto [coords, child] : pendingBirths)
    {
        if (Agents.find(coords) == Agents.end() and
            Plants.find(coords) == Plants.end())
        {

            Agents[coords] = child;
            child->setCoords(coords);
            Agents[coords]->setProcessed(true);
        }
        else
        {
            delete child;
        }
    }

    pendingBirths.clear();
    for (auto agentEntry : Agents)
    {
        agentEntry.second->setProcessed(false);
    }
}
std::vector<std::string> Spider::sortAgentsBySpeed(std::vector<std::string> agents)
{

    std::sort(
        agents.begin(),
        agents.end(),
        [this](const std::string &a, const std::string &b)
        {
            float speedA = Agents.at(a)->getSpeed();
            float speedB = Agents.at(b)->getSpeed();

            return speedA > speedB;
        });

    return agents;
}

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
        return touchSomething(&Agents, &Plants, AgentCoordinates);

    case 1:
        return touchColor(
            &Agents,
            &Plants,
            AgentCoordinates,
            Sense->getUnitColor());

    case 2:
        return energyCountAboveSet(
            Agents.at(AgentCoordinates)->getEnergy(),
            Sense->getSetAmount());

    case 3:
        return energyCountBelowSet(
            Agents.at(AgentCoordinates)->getEnergy(),
            Sense->getSetAmount());

    case 4:
        return healthCountAboveSet(
            Agents.at(AgentCoordinates)->getHealth(),
            Sense->getSetAmount());

    case 5:
        return healthCountBelowSet(
            Agents.at(AgentCoordinates)->getHealth(),
            Sense->getSetAmount());

    case 6:
        return seeSomething(6, &Agents, &Plants, AgentCoordinates);
        break;

    case 7:
        return seeSomething(7, &Agents, &Plants, AgentCoordinates);
        break;

    case 8:
        return seeSomething(8, &Agents, &Plants, AgentCoordinates);
        break;

    case 9:
        return seeSomething(9, &Agents, &Plants, AgentCoordinates);
        break;

    case 10:
        return seeColor(10, &Agents, &Plants, AgentCoordinates, Sense->getUnitColor());
        break;

    case 11:
        return seeColor(11, &Agents, &Plants, AgentCoordinates, Sense->getUnitColor());
        break;

    case 12:
        return seeColor(12, &Agents, &Plants, AgentCoordinates, Sense->getUnitColor());
        break;

    case 13:
        return seeColor(13, &Agents, &Plants, AgentCoordinates, Sense->getUnitColor());
        break;

    default:

        break;
    }

    return false;
}

OutputNode *Spider::getAction(
    std::string AgentCoordinates,
    InputNode *node)
{

    if (node == nullptr)
    {

        return nullptr;
    }

    float activateNode = dist(gen) / 100.0f;

    float weight = node->getWeight();

    if (activateNode >= weight)
    {

        return nullptr;
    }

    if (!manageSense(AgentCoordinates, node))
    {

        return nullptr;
    }

    if (node->getOutputNode() != nullptr and
        node->getOutputNode()->getKey() != 255)
    {

        if (activateNode < node->getOutputNode()->getWeight())
        {

            return node->getOutputNode();
        }
    }

    for (InputNode *child : node->getInputNodes())
    {
        if (child == nullptr)
        {

            continue;
        }

        OutputNode *action =
            getAction(
                AgentCoordinates,
                child);

        if (action != nullptr)
        {

            return action;
        }
    }

    return nullptr;
}

void Spider::manageAction(
    std::string AgentCoordinates,
    OutputNode *ActionNode)
{

    if (AgentCoordinates == "" or
        ActionNode == nullptr)
    {

        return;
    }

    if (Agents.at(AgentCoordinates)->getHealth() <= 0.1)
    {

        return;
    }

    int key = ActionNode->getKey();

    switch (key)
    {
    case 0:

        move(&Agents, AgentCoordinates, 'l');
        break;

    case 1:

        move(&Agents, AgentCoordinates, 'r');
        break;

    case 2:

        move(&Agents, AgentCoordinates, 'u');
        break;

    case 3:

        move(&Agents, AgentCoordinates, 'd');
        break;

    case 4:

        bite(
            &Agents,
            &Plants,
            AgentCoordinates,
            ActionNode->getEnergyCost());
        break;

    case 5:

        splitNewAgent(AgentCoordinates);
        break;

    case 6:

        updateHealth(
            ActionNode->getEnergyCost(),
            Agents[AgentCoordinates]);
        break;

    case 7:

        updateSpeed(
            ActionNode->getEnergyCost(),
            Agents[AgentCoordinates]);
        break;

    case 8:

        biteColor(
            &Agents,
            &Plants,
            AgentCoordinates,
            ActionNode->getUnitColor(),
            ActionNode->getEnergyCost());
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

    bool inside =
        aX >= 0 and
        aX <= (terrariumWidth * agentSize * 2) and
        aY >= 0 and
        aY <= (terrariumHeight * agentSize * 2);

    return inside;
}

void Spider::setNextAgent(
    std::string oldCoords,
    std::string newCoords,
    Agent *Self)
{

    bool spotNotTaken =
        Agents.find(newCoords) == Agents.end() and Plants.find(newCoords) == Plants.end();

    bool notPlant =
        Plants.find(oldCoords) == Plants.end();

    bool insideBorders =
        borderCheck(newCoords);

    if (spotNotTaken and
        notPlant and
        insideBorders)
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
    std::string childCoords =
        getSplitCoords(&Agents, &Plants, ParentCoords);

    if (childCoords == "")
    {

        return;
    }
    Agent *parent = Agents[ParentCoords];

    float newHealth = parent->getHealth() / 2.0f;
    float newEnergy = parent->getEnergy() / 2.0f;
    if (newHealth <= 0.1 or newEnergy <= 0.1)
    {
        return;
    }
    parent->setHealth(
        parent->getHealth() / 2.0f);
    parent->setEnergy(parent->getEnergy() / 2.0f);

    Agent *child = new Agent(
        parent->getIdentifier(),
        parent->getHealth(),
        parent->getEnergy(),
        parent->getPlantDiet(),
        parent->getSpeed(),
        parent->getBrain(),
        radiation,
        gen,
        maxBrainChildNodes,
        maxBrainLevel);
    parent->updateColor();
    pendingBirths.push_back(
        {childCoords, child});
}