#include <iostream>
#include "Environment/environment.h"
#include <random>

namespace Test
{

    static void proximityCheck(Agent a)
    {
        Environment env;
        env.spider.Agents.insert({"12_3", &a});
        env.spider.Agents.insert({"12_4", &a});
        env.spider.Agents.insert({"11_2", &a});
        env.spider.Agents.insert({"110_2", &a});
        env.spider.Agents.insert({"11_3", &a});
        // env.makeWindow();
        std::vector<std::string> cords = env.agentProximityCheck("12_3");
        for (std::string c : cords)
        {
            std::cout << c;
        }
    }

    static void printBrain(InputNode *node, int depth = 0)
    {
        if (!node)
            return;

        // Indent based on depth
        std::string indent(depth * 2, ' ');

        // Print node itself
        std::cout << indent;
        if (node->getKey() != 255)
        {
            std::cout << node->getKey() << "|" << getSenses().at(node->getKey());
        }
        else
        {
            std::cout << node->getKey() << "|INVALID_SENSE_KEY";
        }

        // If it has an output node, show it as a leaf
        if (node->getOutputNode())
        {
            std::cout << "  -> OUT:" << node->getOutputNode()->getKey();
        }
        std::cout << "\n";

        // Recurse to children
        for (InputNode *cn : node->getInputNodes())
        {
            printBrain(cn, depth + 1);
        }
    }

    static void getType(auto *data)
    {
        if (typeid(*data) == typeid(Agent))
        {
            std::cout << "a";
        }
        else if (typeid(*data) == typeid(Plant))
        {
            std::cout << "p";
        }
    }
};
