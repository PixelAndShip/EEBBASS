#include "sim_manager.h"

SimManager::SimManager()
{
    for (int i = 0; i < 3; i++)
    {
        Environment *ce = new Environment();
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
