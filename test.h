#include <iostream>
#include "environment/environment.h"
#include <random>

namespace Test
{

    static void proximityCheck(Agent *a)
    {
        Environment env;
        env.spider.Agents.insert({"12_3", a});
        env.spider.Agents.insert({"12_4", a});
        env.spider.Agents.insert({"11_2", a});
        env.spider.Agents.insert({"110_2", a});
        env.spider.Agents.insert({"11_3", a});
        Plant *pl = new Plant();
        env.spider.Plants.insert({"12_3", pl});
        env.spider.Plants.insert({"12_4", pl});
        env.spider.Plants.insert({"121_3", pl});
        // env.makeWindow();
        std::cout << std::to_string(a->getX()) + "_" + std::to_string(a->getY()) + "\n";
        env.spider.setProximities(std::to_string(a->getX()) + "_" + std::to_string(a->getY()));
        for (auto c : env.spider.proximateCoords)
        {
            if (env.spider.Agents.find(c.first) != env.spider.Agents.end())
            {
                std::cout << "Agent: ";
            }
            else if (env.spider.Plants.find(c.first) != env.spider.Plants.end())
            {
                std::cout << "Plant: ";
            }
            std::cout << c.first;
        }
        std::cout << "\n";
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
