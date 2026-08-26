#include "sim_manager.h"

SimManager::~SimManager()
{
    for (Environment *env : environments)
    {
        delete env;
    }
}

SimManager::SimManager(int threadCount, float eRad, int iT, int maxIT, int maxCYCLE, int cb, int maxBL, int maxBCN, int rootNodesCount, int borderW, int borderH)
{
    if (threadCount > 0)
    {
        for (int i = 0; i < threadCount; i++)
        {
            Environment *ce = new Environment(i, eRad, iT, maxIT, maxCYCLE, cb, maxBL, maxBCN, rootNodesCount, borderW, borderH);
            environments.push_back(ce);
        }
    }
}

void SimManager::runSimulation()
{

    std::vector<std::thread> threads;
    std::cout << "\nSimulation started!\n";
    for (Environment *env : environments)
    {

        threads.emplace_back([env]()
                             { 
        env->setState(EnvironmentState::Running);
        env->manageSimulation();
        env->setState(EnvironmentState::Finished); });
    }
    for (std::thread &thr : threads)
    {
        thr.join();
    }
    std::cout << "\nSimulation finished!\n";
}
