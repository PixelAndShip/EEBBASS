#pragma once
#include <vector>
#include <iostream>
#include <random>
#include "raylib.h"
#include "spider.h"
#include <unistd.h>

enum class EnvironmentState
{
    Running,
    Paused,
    Finished,
    Observed
};

class Environment
{
private:
    int identifier;
    float radiation;
    int maxAgentCount;
    int maxPlantCount;
    float cullPercentage;
    int iteration;
    int maxCultivateIteration;
    int maxCycle;
    int carbon_count;
    int maxRootNodes;
    int maxBrainLevel;
    int maxBrainChildNodes;
    bool custom;

    EnvironmentState environmentState = EnvironmentState::Finished;

public:
    Spider *spider = nullptr;

    Environment(int id = 5000, float eRad = 0.5, int maxAC = 200, int maxPC = 100, float cullP = 0.25, int iT = 0, int maxIT = 115, int maxCYCLE = 100, int cb = 0, int maxBL = 5, int maxBCN = 2, int rootNodesCount = 7, int borderW = borderWidth, int borderH = borderHeight);

    Environment(std::string saveFile);

    ~Environment();

    EnvironmentState getState() const;

    void setState(EnvironmentState iES);

    void manageSimulation();

    void managePlantCount(int count = 100, float cullingCount = 0.25); // makes sure sim does not crash

    void manageAgentCount(int count = 250, float cullingCount = 0.25); // makes sure sim does not crash

    void manageMoment(); /*
    loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
    this means each moment needs to be subdivided into submoments:
    determening which agent gets to perform their external action first is determined by which agent has the higher speed stat

    */
    void manageVisualizedSimulation(int iFPS = 10);
    // void manageSubMoment(std::string coords);

    void cultivateSimulation(int targetPop = 200);

    void makeWindow();

    void spawnUnits(int agentCount = 200, int plantCount = 50);

    void generateAgentCoords(Agent *ag);

    void generatePlantCoords(Plant *pl);

    void logEnvironment(std::string fileName = "");

    void constructEnvironment(std::string fileName);

    void setCustomEnvironmentValues(std::string data);
};