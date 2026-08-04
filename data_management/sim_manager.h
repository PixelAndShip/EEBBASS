#pragma once
#include "environment/environment.h"
#include <fstream>
#include <thread>
class SimManager
{
private:
    std::vector<Environment *> environments;

public:
    SimManager();
    ~SimManager();
    SimManager(int threadCount);
    std::vector<Environment *> &getEnvironments() { return environments; }
    void runSimulation();
};