#include "sim_manager.h"

SimManager::SimManager()
{
    Environment *ce = new Environment();
    currentEnvironment = ce;
    cycle = 0;
}

SimManager::~SimManager()
{
    delete currentEnvironment;
}

void SimManager::logSimCycle()
{
    std::string fileName = "ES_Log_" + std::to_string(cycle) + ".txt";
    std::ofstream logFile(fileName);
    std::string data = "";
    for (auto &a : currentEnvironment->spider->PastAgents)
    {
        std::cout << a.first << '\n';

        std::cout << "Brain roots: "
                  << a.second->getBrain().getInputNodes().size()
                  << '\n';

        for (InputNode *i : a.second->getBrain().getInputNodes())
        {
            std::cout << "Node = " << i << '\n';
            data += outputBrain(i, 0);
        }
    }
    logFile << data;
    logFile.close();
}

std::string SimManager::outputBrain(InputNode *node, int depth = 0)
{
    if (!node)
        return "";

    // Indent based on depth
    std::string data = "";
    std::string indent(depth * 2, ' ');

    // Print node itself
    data += indent;
    if (node->getKey() != 255)
    {
        data += std::to_string(node->getKey()) + "|" + getSenses().at(node->getKey()) + "|" + std::to_string(node->getWeight());
    }
    else
    {
        data += std::to_string(node->getKey()) + "|INVALID_SENSE_KEY";
    }

    // If it has an output node, show it as a leaf
    if (node->getOutputNode())
    {
        data += "---" + std::to_string(node->getOutputNode()->getKey()) + "|" + getActions().at(node->getOutputNode()->getKey()) + "|" + std::to_string(node->getOutputNode()->getWeight());
    }
    data += "\n";

    // Recurse to children
    for (InputNode *cn : node->getInputNodes())
    {
        data += outputBrain(cn, depth + 1);
    }
    return data;
}