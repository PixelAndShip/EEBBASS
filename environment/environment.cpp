#include "environment.h"
#include <vector>

Environment::Environment()
{

    radiation = 0.5;
    carbon_count = 1;
    maxBrainLevel = 5;
    maxBrainChildNodes = 2;
    spider = new Spider();
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> d(0, 7);
    std::uniform_int_distribution<> insideBorders(0, 800);
    gen = g;
    dist = d;
}

void Environment::manageSimulation()
{
}

void Environment::managePlantCount()
{
} // makes sure sim does not crash

void Environment::manageAgentCount() {} // makes sure sim does not crash

void Environment::manageMoment()
{
    /*
    loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
    this means each moment needs to be subdivided into submoments:
    determening which agent gets to perform their external action first is determined by which agent has the higher speed stat
    */
    std::string agentCoords = "";

    for (auto cs : spider->PastAgents)
    {
        agentCoords = cs.first;

        manageSubMoment(agentCoords);
    }
    /*
        for(auto pa : spider->PastAgents){
            spider->setNextAgent(pa.first,pa.second);
        }
    */
}

void Environment::manageSubMoment(std::string coords)
{

    // std::cout << "initiated manageSubMoment \n";
    // go through sorounding agent brains and determine first actions, add managed to managedAgentCoordinates

    if (spider->proximateCoords.find(coords) == spider->proximateCoords.end() and coords != "")
    {
        // std::cout << "?" << coords << "?";
        spider->proximateCoords.clear();
        spider->proximateCoords[coords] = true;
        spider->setProximities(coords);
        spider->manageSubMoment();
    }

    // std::cout << "finishedSetProximites and manageSubmomentSpider \n";
}

void Environment::makeWindow()
{
    InitWindow(800, 800, "Environment");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto ac : spider->PastAgents)
        {
            DrawCircle(
                ac.second->getX(),
                ac.second->getY(),
                20,
                (Color){
                    (unsigned char)ac.second->getAgentColor().red,
                    (unsigned char)ac.second->getAgentColor().green,
                    (unsigned char)ac.second->getAgentColor().blue,
                    (unsigned char)ac.second->getAgentColor().transparency});
        }

        EndDrawing();
        manageMoment();
    }
    CloseWindow();
}

void Environment::startSimulation()
{
    int AgentCount = 8; // temporary

    for (int i = 0; i < AgentCount; i++)
    {
        Agent *genesis = new Agent(radiation, gen, dist, maxBrainChildNodes, maxBrainLevel);
        generateCoords(genesis);
        spider->PastAgents[genesis->getCoords()] = genesis;
    }
}

void Environment::generateCoords(Agent *ag)
{
    int x = insideBorders(gen);
    int y = insideBorders(gen);
    for (auto ac : spider->PastAgents)
    {
        if (ac.second->getX() == x and ac.second->getY() == y)
        {
            generateCoords(ag);
            return;
        }
    }
    ag->setX(x);
    ag->setY(y);
}