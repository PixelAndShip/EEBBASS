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
    int maxCultivateIteration;
    int maxCycle;
    int carbon_count;
    int maxRootNodes;
    int maxBrainLevel;
    int maxBrainChildNodes;
    bool custom;

public:
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    std::uniform_int_distribution<> insideX;
    std::uniform_int_distribution<> insideY;

    Spider *spider = nullptr;

    // identifier = id;
    // radiation = 0.5;
    // iteration = 0;
    // carbon_count = 1;

    // maxBrainLevel = 5;
    // maxBrainChildNodes = 2;

    // spider = new Spider();

    // std::random_device rd;
    // std::mt19937 g(rd());

    // std::uniform_int_distribution<> d(0, 7);
    // std::uniform_int_distribution<> iB(0, 100);

    // gen = g;
    // dist = d;
    // insideBorders = iB;
    // Environment(int id = 5000);
    Environment(int id = 5000, float eRad = 0.5, int iT = 0, int maxIT = 115, int maxCYCLE = 100, int cb = 0, int maxBL = 5, int maxBCN = 2, int rootNodesCount = 7, int borderW = borderWidth, int borderH = borderHeight);

    Environment(std::string saveFile);

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

    void logEnvironment(std::string fileName = "");

    void constructEnvironment(std::string fileName);

    void setCustomEnvironmentValues(std::string data);
};