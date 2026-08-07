#include "sim_manager.h"

SimManager::~SimManager()
{
    for (Environment *env : environments)
    {
        delete env;
    }
}

SimManager::SimManager(int threadCount, float eRad, int iT, int maxIT, int maxCYCLE, int cb, int maxBL, int maxBCN, int rootNodesCount, int borderSize)
{
    if (threadCount > 0)
    {
        for (int i = 0; i < threadCount; i++)
        {
            Environment *ce = new Environment(i, eRad, iT, maxIT, maxCYCLE, cb, maxBL, maxBCN, rootNodesCount, borderSize);
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
                             { env->manageSimulation(); });
    }
    for (std::thread &thr : threads)
    {
        thr.join();
    }
    std::cout << "\nSimulation finished!\n";
}
