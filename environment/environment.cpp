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

    gen = g;
    dist = d;
}

void Environment::manageSimulation()
{
}

void Environment::managePlantCount() {} // makes sure sim does not crash

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
        manageMoment(); // implement
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
    }
    CloseWindow();
}