#include "sim_manager.h"

SimManager::~SimManager()
{
    requestStop();
    waitForSimulation();
    for (Environment *env : environments)
    {
        delete env;
    }
}

SimManager::SimManager(int threadCount, float eRad, int maxAC, int maxPC, float cullP, int iT, int maxIT, int maxCYCLE, int cb, int maxBL, int maxBCN, int rootNodesCount, int borderW, int borderH)
{
    if (threadCount > 0)
    {
        for (int i = 0; i < threadCount; i++)
        {
            Environment *ce = new Environment(i, eRad, maxAC, maxPC, cullP, iT, maxIT, maxCYCLE, cb, maxBL, maxBCN, rootNodesCount, borderW, borderH);
            environments.push_back(ce);
        }
    }
}

void SimManager::startSimulation()
{
    if (started)
    {
        return;
    }
    started = true;
    finishedCount = 0;

    for (Environment *env : environments)
    {
        threads.emplace_back([this, env]()
                             {
        env->setState(EnvironmentState::Running);
        env->manageSimulation();
        env->setState(EnvironmentState::Finished);
        finishedCount++; });
    }
}

bool SimManager::isRunning() const
{
    return started and finishedCount < (int)environments.size();
}

void SimManager::requestStop()
{
    for (Environment *env : environments)
    {
        env->requestStop();
    }
}

void SimManager::waitForSimulation()
{
    for (std::thread &thr : threads)
    {
        if (thr.joinable())
        {
            thr.join();
        }
    }
    threads.clear();
}

void SimManager::runSimulation()
{
    std::cout << "\nSimulation started!\n";
    startSimulation();
    waitForSimulation();
    std::cout << "\nSimulation finished!\n";
}
