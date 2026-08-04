#pragma once
#include <vector>
#include <iostream>
#include <random>
#include "raylib.h"
#include "spider.h"
#include <unistd.h>
class Environment
{
private:
    int identifier;
    float radiation;
    int iteration;
    int carbon_count;
    int maxBrainLevel;
    int maxBrainChildNodes;

public:
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    std::uniform_int_distribution<> insideBorders;

    Spider *spider = nullptr;

    Environment();
    Environment(int id);
    ~Environment();

    void manageSimulation();

    void managePlantCount(int count = 100, float cullingCount = 0.25); // makes sure sim does not crash

    void manageAgentCount(int count = 250, float cullingCount = 0.25); // makes sure sim does not crash

    void manageMoment(); /*
    loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
    this means each moment needs to be subdivided into submoments:
    determening which agent gets to perform their external action first is determined by which agent has the higher speed stat

    */
    void manageVisualizedSimulation();
    // void manageSubMoment(std::string coords);

    void cultivateSimulation(int targetPop = 200);

    void makeWindow();

    void startSimulation(int agentCount = 200, int plantCount = 50);

    void generateAgentCoords(Agent *ag);

    void generatePlantCoords(Plant *pl);

    void customizeAgent();
};