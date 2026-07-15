#include "environment.h"
#include <vector>

Environment::Environment()
{
    DEBUG_LOG("Starting Environment constructor");

    radiation = 0.5;
    carbon_count = 1;

    maxBrainLevel = 5;
    maxBrainChildNodes = 2;

    DEBUG_LOG("Environment settings initialized");
    DEBUG_LOG("Radiation: " << radiation);
    DEBUG_LOG("Carbon count: " << carbon_count);
    DEBUG_LOG("Max brain level: " << maxBrainLevel);
    DEBUG_LOG("Max brain child nodes: " << maxBrainChildNodes);

    DEBUG_LOG("Creating Spider");

    spider = new Spider();

    std::random_device rd;
    std::mt19937 g(rd());

    std::uniform_int_distribution<> d(0, 7);
    std::uniform_int_distribution<> iB(0, 40);

    gen = g;
    dist = d;
    insideBorders = iB;

    DEBUG_LOG("Random generators initialized");

    DEBUG_LOG("Finished Environment constructor");
}

Environment::~Environment()
{
    DEBUG_LOG("Starting Environment destructor");

    DEBUG_LOG("Deleting Spider");

    delete spider;

    DEBUG_LOG("Finished Environment destructor");
}

void Environment::manageSimulation()
{
    DEBUG_LOG("Managing simulation");
}

void Environment::managePlantCount()
{
    DEBUG_LOG("Managing plant count");
}

void Environment::manageAgentCount()
{
    DEBUG_LOG("Managing agent count");
}

void Environment::manageMoment()
{
    DEBUG_LOG("Starting simulation moment");

    std::string agentCoords = "";

    DEBUG_LOG("Processing "
              << spider->Agents.size()
              << " agents");

    std::vector<std::string> coords;

    for (auto &[c, a] : spider->Agents)
        coords.push_back(c);

    for (auto &c : coords)
    {
        DEBUG_LOG("Processing Agent at "
                  << c);
        if (spider->Agents.find(c) != spider->Agents.end())
            manageSubMoment(c);
    }

    DEBUG_LOG("Applying passive health drain");

    Agent *agent = nullptr;
    for (auto it = spider->Agents.begin();
         it != spider->Agents.end();)
    {
        agent = it->second;

        if (agent == nullptr)
        {
            DEBUG_LOG("Found null Agent, removing");
            it = spider->Agents.erase(it);
            continue;
        }

        float oldHealth = agent->getHealth();

        agent->setHealth(
            oldHealth - 0.1);

        DEBUG_LOG("Agent "
                  << it->first
                  << " health "
                  << oldHealth
                  << " -> "
                  << agent->getHealth());

        if (agent->getHealth() <= 0.001)
        {
            DEBUG_LOG("Agent died at "
                      << it->first);

            delete agent;

            it = spider->Agents.erase(it);
        }
        else
        {
            DEBUG_LOG("Updating Agent color");

            agent->updateColor();

            ++it;
        }
    }

    DEBUG_LOG("Finished simulation moment");
}

void Environment::manageSubMoment(std::string coords)
{
    DEBUG_LOG("Managing sub moment for "
              << coords);

    if (coords == "")
    {
        DEBUG_LOG("Invalid empty coordinates");
        return;
    }

    if (spider->proximateCoords.find(coords) ==
        spider->proximateCoords.end())
    {
        DEBUG_LOG("Coordinates not initialized in proximity map");

        spider->proximateCoords.clear();

        spider->proximateCoords[coords] = true;

        DEBUG_LOG("Setting proximities");

        spider->setProximities(coords);

        DEBUG_LOG("Managing Spider sub moment");

        spider->manageSubMoment();
    }
    else
    {
        DEBUG_LOG("Coordinates already processed");
    }

    DEBUG_LOG("Finished sub moment for "
              << coords);
}

void Environment::makeWindow()
{
    DEBUG_LOG("Creating window");

    InitWindow(800, 800, "Environment");

    SetTargetFPS(5);

    DEBUG_LOG("Window initialized");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DEBUG_LOG("Drawing "
                  << spider->Agents.size()
                  << " agents");

        for (auto ac : spider->Agents)
        {
            if (ac.second == nullptr)
            {
                DEBUG_LOG("Skipping null Agent");
                continue;
            }

            UnitColor color =
                ac.second->getAgentColor();

            DrawCircle(
                ac.second->getX(),
                ac.second->getY(),
                10,
                (Color){
                    (unsigned char)color.red,
                    (unsigned char)color.green,
                    (unsigned char)color.blue,
                    (unsigned char)color.transparency});
        }

        EndDrawing();

        DEBUG_LOG("Updating simulation");

        manageMoment();
    }

    DEBUG_LOG("Closing window");

    CloseWindow();

    DEBUG_LOG("Window closed");
}

void Environment::startSimulation()
{
    DEBUG_LOG("Starting simulation");

    int AgentCount = 8;

    DEBUG_LOG("Creating "
              << AgentCount
              << " starting agents");

    for (int i = 0; i < AgentCount; i++)
    {
        DEBUG_LOG("Creating genesis Agent "
                  << i);

        Agent *genesis =
            new Agent(
                radiation,
                gen,
                dist,
                maxBrainChildNodes,
                maxBrainLevel);

        DEBUG_LOG("Generating coordinates");

        generateCoords(genesis);

        DEBUG_LOG("Adding Agent at "
                  << genesis->getCoords());

        spider->Agents[genesis->getCoords()] =
            genesis;
    }

    DEBUG_LOG("Starting window");

    makeWindow();
}

void Environment::generateCoords(Agent *ag)
{
    DEBUG_LOG("Generating coordinates for Agent");

    if (ag == nullptr)
    {
        DEBUG_LOG("Cannot generate coordinates for null Agent");
        return;
    }

    int x = insideBorders(gen) * 20;
    int y = insideBorders(gen) * 20;

    DEBUG_LOG("Generated coordinates "
              << x
              << "_"
              << y);

    for (auto ac : spider->Agents)
    {
        if (ac.second == nullptr)
        {
            continue;
        }

        if (ac.second->getX() == x and
            ac.second->getY() == y)
        {
            DEBUG_LOG("Coordinates occupied by Agent at "
                      << ac.first);

            generateCoords(ag);

            return;
        }
    }

    DEBUG_LOG("Assigning coordinates "
              << x
              << "_"
              << y);

    ag->setX(x);
    ag->setY(y);

    DEBUG_LOG("Finished coordinate generation");
}