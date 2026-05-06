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
    std::unordered_map<std::string, bool> managedAgentCoordinates = {};
    for (std::string coords : spider.proximateAgentCoords)
    {

        if (managedAgentCoordinates.find(coords) != managedAgentCoordinates.end())
        {
            continue;
        }

        manageSubMoment(coords, &managedAgentCoordinates);
    }
}

void Environment::manageSubMoment(std::string coords, std::unordered_map<std::string, bool> *managedAgentCoordinates)
{
    if (managedAgentCoordinates == nullptr)
    {
        return;
    }
    // go through sorounding agent brains and determine first actions, add managed to managedAgentCoordinates
    spider.setProximities(coords);

    for (std::string agentCoords : spider.proximateAgentCoords)
    {
        managedAgentCoordinates->insert({agentCoords, true});
    }
    spider.proximateAgentCoords.clear();
    spider.proximatePlantCoords.clear();
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