#pragma once
#include "environment/environment.h"
#include <fstream>
class SimManager
{
private:
    Environment *currentEnvironment;
    int cycle;

public:
    SimManager();
    ~SimManager();
    Environment *getEnvironment() { return currentEnvironment; };
    int getCycle() { return cycle; };
    void setEnvironment(Environment *en)
    {
        currentEnvironment = en;
    }
    void setCycle(int c) { cycle = c; };
    void logSimCycle();
    std::string outputBrain(InputNode *node, int depth);
};