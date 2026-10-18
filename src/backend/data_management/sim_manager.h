#pragma once
#include "src/backend/core/environment.h"
#include <fstream>
#include <thread>
#include <atomic>
class SimManager
{
private:
    std::vector<Environment *> environments;
    std::vector<std::thread> threads;
    std::atomic<int> finishedCount{0};
    bool started = false;

public:
    ~SimManager();
    SimManager(int threadCount = 4, float eRad = 0.5, int maxAC = 200, int maxPC = 100, float cullP = 0.25, int iT = 0, int maxIT = 120, int maxCYCLE = 100, int cb = 0, int maxBL = 5, int maxBCN = 2, int rootNodesCount = 7, int borderW = borderWidth, int borderH = borderHeight);
    std::vector<Environment *> &getEnvironments()
    {
        return environments;
    }

    // Blocking: starts every environment on its own thread and waits for all.
    void runSimulation();

    // Non-blocking variant for a frontend: start, poll isRunning() /
    // getFinishedCount(), then waitForSimulation() to join the threads.
    void startSimulation();
    bool isRunning() const;
    int getFinishedCount() const { return finishedCount; }
    int getEnvironmentCount() const { return (int)environments.size(); }
    void requestStop();
    void waitForSimulation();
};
