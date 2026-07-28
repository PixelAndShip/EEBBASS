#include "environment.h"
#include <vector>

Environment::Environment()
{

    identifier = 0;
    radiation = 0.5;
    iteration = 0;
    carbon_count = 1;

    maxBrainLevel = 5;
    maxBrainChildNodes = 2;

    spider = new Spider();

    std::random_device rd;
    std::mt19937 g(rd());

    std::uniform_int_distribution<> d(0, 7);
    std::uniform_int_distribution<> iB(0, 100);

    gen = g;
    dist = d;
    insideBorders = iB;
}

Environment::~Environment()
{

    delete spider;
    std::stringstream writtenData;
    std::string fileName = "logs/Agent_Brain_Log_" + std::to_string(identifier) + ".txt";
    std::ifstream CurrentLog(fileName);
    if (CurrentLog)
    {
        writtenData << CurrentLog.rdbuf();
    }
    std::string data = writtenData.str();
    CurrentLog.close();
    std::ofstream endOfSim(fileName);
    endOfSim << data + "\n=========================================\n";
    endOfSim.close();
}

void Environment::manageSimulation()
{

    startSimulation();
    InitWindow(800, 800, "Environment");
    SetTargetFPS(4);
    int AgentsCount = 0;
    int PlantsCount = 0;
    std::string text = "";
    while (!WindowShouldClose())
    {
        AgentsCount = (int)spider->Agents.size();
        PlantsCount = (int)spider->Plants.size();
        text = std::to_string(AgentsCount) + "|" + std::to_string(PlantsCount) + "|" + std::to_string(iteration);
        if (iteration >= 120)
        {
            iteration = 0;
            cultivateSimulation();
        }
        manageMoment();
        BeginDrawing();
        ClearBackground(BLACK);
        makeWindow();
        DrawText(text.c_str(), 20, 20, 30, WHITE);
        EndDrawing();

        iteration++;
    }

    CloseWindow();
}

void Environment::cultivateSimulation(int targetPop)
{

    std::vector<std::pair<std::string, float>> rankedAgents;

    for (auto &[coords, agent] : spider->Agents)
    {
        if (agent == nullptr)
            continue;

        float fitness = agent->getHealth() + agent->getEnergy();
        rankedAgents.push_back({coords, fitness});
    }

    std::sort(rankedAgents.begin(), rankedAgents.end(),
              [](const auto &a, const auto &b)
              {
                  return a.second > b.second;
              });

    int originalPopulation = rankedAgents.size();
    int survivors = originalPopulation / 2;

    for (int i = 0; i < survivors; i++)
    {
        Agent *agent = spider->Agents[rankedAgents[i].first];

        if (agent != nullptr)
        {
            agent->setHealth(100);
            agent->setEnergy(100);
        }
        agent->getBrain().logBrain();
    }

    for (int i = survivors; i < originalPopulation; i++)
    {
        auto it = spider->Agents.find(rankedAgents[i].first);

        if (it != spider->Agents.end())
        {
            delete it->second;
            spider->Agents.erase(it);
        }
    }
    int plantTargetPop = 150 - (int)spider->Plants.size();
    int agentTargetPop = targetPop - survivors;
    if (plantTargetPop <= 0)
    {
        plantTargetPop = 50;
    }
    if (targetPop - survivors <= 0)
    {
        agentTargetPop = 100;
    }
    startSimulation(
        agentTargetPop, plantTargetPop);
}

void Environment::managePlantCount(int count, float cullingCount)
{
}

void Environment::manageAgentCount(int count, float cullingCount)
{
    int maxAgentCount = count;
    if (spider->Agents.size() > maxAgentCount)
    {
        int cL = spider->Agents.size() * cullingCount;

        std::vector<std::string> ids;

        for (auto &[id, agent] : spider->Agents)
        {
            if (agent != nullptr)
            {
                ids.push_back(id);
            }
        }

        std::shuffle(
            ids.begin(),
            ids.end(),
            gen);

        for (int i = 0; i < cL and i < ids.size(); i++)
        {
            auto it = spider->Agents.find(ids[i]);

            if (it != spider->Agents.end())
            {
                delete it->second;
                spider->Agents.erase(it);
            }
        }
    }
}

void Environment::manageMoment()
{

    Agent *agent = nullptr;

    for (auto it = spider->Agents.begin();
         it != spider->Agents.end();)
    {
        agent = it->second;

        if (agent == nullptr)
        {
            it = spider->Agents.erase(it);
            continue;
        }

        float oldHealth = agent->getHealth();
        float oldEnergy = agent->getEnergy();
        agent->setHealth(
            oldHealth - 1);
        agent->setEnergy(oldEnergy - 1);
        if (agent->getEnergy() <= 0.1)
        {
            agent->setEnergy(0);
            agent->setHealth(
                agent->getHealth() - 1);
        }
        if (agent->getHealth() <= 0.1)
        {
            delete agent;
            it = spider->Agents.erase(it);
        }
        else
        {
            agent->updateColor();
            ++it;
        }
    }
    Plant *plant = nullptr;
    for (auto pl = spider->Plants.begin();
         pl != spider->Plants.end();)
    {
        plant = pl->second;

        if (plant == nullptr)
        {
            pl = spider->Plants.erase(pl);
            continue;
        }

        float oldPlantHealth = plant->getHealth();

        plant->setHealth(
            oldPlantHealth - 1);

        if (plant->getHealth() <= 0.1)
        {

            delete plant;

            pl = spider->Plants.erase(pl);
        }
        else
        {
            plant->updateColor();
            ++pl;
        }
    }

    spider->manageSubMoment();
    manageAgentCount();
}

// void Environment::manageSubMoment(std::string coords)
// {
//     DEBUG_LOG("Managing sub moment for "
//               << coords);

//     if (coords == "")
//     {
//         DEBUG_LOG("Invalid empty coordinates");
//         return;
//     }

//     if (spider->proximateCoords.find(coords) ==
//         spider->proximateCoords.end())
//     {
//         DEBUG_LOG("Coordinates not initialized in proximity map");

//         spider->proximateCoords.clear();

//         spider->proximateCoords[coords] = true;

//         DEBUG_LOG("Setting proximities");

//         spider->setProximities(coords);

//         DEBUG_LOG("Managing Spider sub moment");

//         spider->manageSubMoment();
//     }
//     else
//     {
//         DEBUG_LOG("Coordinates already processed");
//     }

//     DEBUG_LOG("Finished sub moment for "
//               << coords);
// }
void Environment::makeWindow()
{

    for (auto &[coords, agent] : spider->Agents)
    {
        if (!agent)
        {
            continue;
        }
        UnitColor color = agent->getAgentColor();

        DrawCircle(
            agent->getX(),
            agent->getY(),
            4,
            {(unsigned char)color.red,
             (unsigned char)color.green,
             (unsigned char)color.blue,
             (unsigned char)color.transparency});
    }
    for (auto &[coords, plant] : spider->Plants)
    {
        if (!plant)
        {
            continue;
        }
        UnitColor plC = plant->getPlantColor();
        DrawCircle(
            plant->getX(),
            plant->getY(),
            3,
            {(unsigned char)plC.red,
             (unsigned char)plC.green,
             (unsigned char)plC.blue,
             (unsigned char)plC.transparency});
    }
}

void Environment::startSimulation(int agentCount, int plantCount)
{
    int AgentCount = agentCount;
    int PlantCount = plantCount;
    for (int i = 0; i < AgentCount; i++)
    {
        Agent *genesis =
            new Agent(
                identifier,
                radiation,
                gen,
                dist,
                maxBrainChildNodes,
                maxBrainLevel);
        generateAgentCoords(genesis);
        if (genesis->getX() == 1000)
        {
            delete genesis;
            continue;
        }
        spider->Agents[genesis->getCoords()] =
            genesis;
    }

    for (int j = 0; j < PlantCount; j++)
    {
        Plant *pl = new Plant(radiation, gen);
        generatePlantCoords(pl);
        if (pl->getX() == 1000)
        {
            delete pl;
            continue;
        }
        spider->Plants[pl->getCoords()] = pl;
    }
}

void Environment::generateAgentCoords(Agent *ag)
{

    int cycles = 0;
    if (ag == nullptr)
    {

        return;
    }
    while (true)
    {
        if (cycles >= 300)
        {

            return;
        }
        int x = insideBorders(gen) * 8;
        int y = insideBorders(gen) * 8;
        bool occupied = false;
        for (auto &ac : spider->Agents)
        {
            if (ac.second == nullptr)
            {
                continue;
            }

            if (ac.second->getX() == x and
                ac.second->getY() == y)
            {
                occupied = true;
                break;
            }
        }
        if (occupied == false)
        {
            for (auto &pl : spider->Plants)
            {
                if (pl.second == nullptr)
                {
                    continue;
                }

                if (pl.second->getX() == x and
                    pl.second->getY() == y)
                {
                    occupied = true;
                    break;
                }
            }
        }

        if (occupied == false)
        {

            ag->setX(x);
            ag->setY(y);

            return;
        }
        cycles += 1;
    }
}

void Environment::generatePlantCoords(Plant *pl)
{

    int cycles = 0;
    if (pl == nullptr)
    {

        return;
    }
    while (true)
    {
        if (cycles >= 300)
        {

            return;
        }
        int x = insideBorders(gen) * 8;
        int y = insideBorders(gen) * 8;

        bool occupied = false;
        for (auto &ac : spider->Agents)
        {
            if (ac.second == nullptr)
            {
                continue;
            }

            if (ac.second->getX() == x and
                ac.second->getY() == y)
            {
                occupied = true;
                break;
            }
        }
        if (occupied == false)
        {
            for (auto &pl : spider->Plants)
            {
                if (pl.second == nullptr)
                {
                    continue;
                }

                if (pl.second->getX() == x and
                    pl.second->getY() == y)
                {
                    occupied = true;
                    break;
                }
            }
        }

        if (occupied == false)
        {

            pl->setX(x);
            pl->setY(y);

            return;
        }
        cycles += 1;
    }
}
