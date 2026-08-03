#include "sim_manager.h"

SimManager::SimManager()
{
    for (int i = 0; i < 4; i++)
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

void SimManager::runSimulation()
{
    std::vector<std::thread> threads;
    for (Environment *env : environments)
    {

        threads.emplace_back([env]()
                             { env->manageSimulation(); });
    }
    for (std::thread &thr : threads)
    {
        thr.join();
    }
}
