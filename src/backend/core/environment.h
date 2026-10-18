

#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <atomic>
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
    int worldWidth;
    int worldHeight;
    bool custom;

    std::atomic<EnvironmentState> environmentState{EnvironmentState::Finished};
    std::atomic<int> completedCycles{0};
    std::atomic<bool> stopRequested{false};

public:
    Spider *spider = nullptr;

    Environment(int id = 5000, float eRad = 0.5, int maxAC = 200, int maxPC = 100, float cullP = 0.25, int iT = 0, int maxIT = 115, int maxCYCLE = 100, int cb = 0, int maxBL = 5, int maxBCN = 2, int rootNodesCount = 7, int borderW = borderWidth, int borderH = borderHeight);

    Environment(std::string saveFile);

    ~Environment();

    EnvironmentState getState() const;
    int getIdentifier() const { return identifier; }
    int getIteration() const { return iteration; }
    int getMaxCultivateIteration() const { return maxCultivateIteration; }
    int getCompletedCycles() const { return completedCycles; }
    int getMaxCycle() const { return maxCycle; }
    int getWorldWidth() const { return worldWidth; }
    int getWorldHeight() const { return worldHeight; }
    int getAgentCount() const { return spider ? (int)spider->Agents.size() : 0; }
    int getPlantCount() const { return spider ? (int)spider->Plants.size() : 0; }
    float getRadiation() const { return radiation; }
    float getCullPercentage() const { return cullPercentage; }
    bool isCustom() const { return custom; }

    void setState(EnvironmentState iES);

    void requestStop() { stopRequested = true; }

    void manageSimulation();

    void start();
    void step();

    void managePlantCount(int count = 100, float cullingCount = 0.25); // makes sure sim does not crash

    void manageAgentCount(int count = 250, float cullingCount = 0.25); // makes sure sim does not crash

    void manageMoment();

    void cultivateSimulation(int targetPop = 200);

    void spawnUnits(int agentCount = 200, int plantCount = 50);

    void generateAgentCoords(Agent *ag);

    void generatePlantCoords(Plant *pl);

    void logEnvironment(std::string fileName = "");

    void constructEnvironment(std::string fileName);

    void setCustomEnvironmentValues(std::string data);
};
