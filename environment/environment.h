#pragma once
#include <vector>
#include <iostream>
#include <random>
#include "raylib.h"
#include "spider.h"
#include <unistd.h>
class Environment
{
public:
    int identifier;
    float radiation;
    int iteration;
    int carbon_count;
    int maxBrainLevel;
    int maxBrainChildNodes;

    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    std::uniform_int_distribution<> insideBorders;

    Spider *spider = nullptr;

    Environment();
    ~Environment();

    void manageSimulation();

    void managePlantCount(); // makes sure sim does not crash

    void manageAgentCount(); // makes sure sim does not crash

    void manageMoment(); /*
    loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
    this means each moment needs to be subdivided into submoments:
    determening which agent gets to perform their external action first is determined by which agent has the higher speed stat

    */

    // void manageSubMoment(std::string coords);

    void makeWindow();

    void startSimulation();

    void generateAgentCoords(Agent *ag);

    void generatePlantCoords(Plant *pl);
};