#include "sim_manager.h"

SimManager::SimManager()
{
    for (int i = 0; i < 8; i++)
    {
        Environment *ce = new Environment(i);
        environments.push_back(ce);
    }
}

SimManager::~SimManager()
{
    for (Environment *env : environments)
    {
        delete env;
    }
}

SimManager::SimManager(int threadCount)
{
    if (threadCount > 0)
    {
        for (int i = 0; i < threadCount; i++)
        {
            Environment *ce = new Environment(i);
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
