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

// void SimManager::logSimCycle()
// {
//     DEBUG_LOG("Starting simulation cycle logging");

//     std::string fileName =
//         "ES_Log_" + std::to_string(cycle) + ".txt";

//     DEBUG_LOG("Creating log file: "
//               << fileName);

//     std::ofstream logFile(fileName);

//     if (!logFile.is_open())
//     {
//         DEBUG_LOG("Failed to open log file");
//         return;
//     }

//     std::string data = "";

//     DEBUG_LOG("Logging "
//               << currentEnvironment->spider->Agents.size()
//               << " agents");

//     for (auto &a : currentEnvironment->spider->Agents)
//     {
//         DEBUG_LOG("Logging Agent at "
//                   << a.first);

//         std::cout << a.first << '\n';

//         if (a.second == nullptr)
//         {
//             DEBUG_LOG("Skipping null Agent");
//             continue;
//         }

//         std::cout << "Brain roots: "
//                   << a.second->getBrain().getInputNodes().size()
//                   << '\n';

//         DEBUG_LOG("Brain has "
//                   << a.second->getBrain().getInputNodes().size()
//                   << " root nodes");

//         for (InputNode *i :
//              a.second->getBrain().getInputNodes())
//         {
//             DEBUG_LOG("Outputting brain node "
//                       << i);

//             std::cout << "Node = "
//                       << i
//                       << '\n';

//             data += outputBrain(i, 0);
//         }

//         data += "\n";
//     }

//     DEBUG_LOG("Writing data to file");

//     logFile << data;

//     logFile.close();

//     DEBUG_LOG("Finished simulation cycle logging");
// }

// std::string SimManager::outputBrain(InputNode *node, int depth)
// {
//     DEBUG_LOG("Outputting brain node at depth "
//               << depth
//               << " pointer "
//               << node);

//     if (!node)
//     {
//         DEBUG_LOG("Node is null");

//         return "";
//     }

//     std::string data = "";

//     std::string indent(
//         depth * 2,
//         ' ');

//     data += indent;

//     unsigned int key = node->getKey();

//     DEBUG_LOG("Node key: "
//               << key);

//     if (key != 255)
//     {
//         DEBUG_LOG("Valid sense key");

//         data +=
//             std::to_string(key) + "|" + getSenses().at(key) + "|" + std::to_string(node->getWeight());
//     }
//     else
//     {
//         DEBUG_LOG("Invalid sense key");

//         data +=
//             std::to_string(key) + "|INVALID_SENSE_KEY";
//     }

//     if (node->getOutputNode())
//     {
//         DEBUG_LOG("Node has OutputNode");

//         unsigned int outputKey =
//             node->getOutputNode()->getKey();

//         DEBUG_LOG("Output key: "
//                   << outputKey);

//         if (outputKey < getActions().size())
//         {
//             data +=
//                 "---" + std::to_string(outputKey) + "|" + getActions().at(outputKey) + "|" + std::to_string(node->getOutputNode()->getWeight());
//         }
//         else
//         {
//             DEBUG_LOG("Invalid output key");

//             data +=
//                 "---" + std::to_string(outputKey) + "|INVALID_ACTION_KEY";
//         }
//     }

//     data += "\n";

//     DEBUG_LOG("Processing "
//               << node->getInputNodes().size()
//               << " child nodes");

//     for (InputNode *cn :
//          node->getInputNodes())
//     {
//         data += outputBrain(
//             cn,
//             depth + 1);
//     }

//     DEBUG_LOG("Finished outputting node at depth "
//               << depth);

//     return data;
// }