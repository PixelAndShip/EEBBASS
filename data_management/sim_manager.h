#pragma once
#include "environment/environment.h"
#include <fstream>
#include <thread>
class SimManager
{
private:
    std::vector<Environment *> environments;

public:
    ~SimManager();
    SimManager(int threadCount = 4, float eRad = 0.5, int iT = 0, int maxIT = 120, int maxCYCLE = 100, int cb = 0, int maxBL = 5, int maxBCN = 2, int rootNodesCount = 7, int borderW = borderWidth, int borderH = borderHeight);
    std::vector<Environment *> &getEnvironments()
    {
        return environments;
    }
    void runSimulation();
};