#include "environment.h"
#include <vector>

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

    std::unordered_map<std::string, bool> managedAgentCoordinates = {};
    std::unordered_map<std::string, Agent *> currentAgents = spider.Agents;
    for (auto cs : currentAgents)
    {
        agentCoords = cs.first;

        if (managedAgentCoordinates.find(agentCoords) == managedAgentCoordinates.end())
        {
            manageSubMoment(agentCoords, &managedAgentCoordinates);
        }
    }
}

void Environment::manageSubMoment(std::string coords, std::unordered_map<std::string, bool> *managedAgentCoordinates)
{

    if (managedAgentCoordinates == nullptr)
    {
        return;
    }
    // std::cout << "initiated manageSubMoment \n";
    // go through sorounding agent brains and determine first actions, add managed to managedAgentCoordinates
    spider.setProximities(coords);
    spider.manageSubMoment();
    // std::cout << "finishedSetProximites and manageSubmomentSpider \n";
    for (auto coords : spider.proximateCoords)
    {
        managedAgentCoordinates->insert({coords.first, true});
    }
    spider.proximateCoords.clear();
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
        DrawCircle(100, 200, 10, RED);

        EndDrawing();
    }
    CloseWindow();
}