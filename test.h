#include <iostream>
#include "Environment/environment.h"
#include <random>

class Test{
public:
static void proximityCheck(Agent a){
    Environment env;
    env.agents.insert({"12_3",&a});
    env.agents.insert({"12_4",&a});
    env.agents.insert({"11_2",&a});
    env.agents.insert({"110_2",&a});
    env.agents.insert({"11_3",&a});
    //env.makeWindow();
    std::vector<std::string> cords = env.agentProximityCheck("12_3");
    for (std::string c : cords){
        std::cout<<c;
    }
}

static void printBrain(InputNode* node, int depth = 0) {
    if (!node) return;

    // Indent based on depth
    std::string indent(depth * 2, ' ');

    // Print node itself
    std::cout << indent;
    if (node->key != 255) {
        std::cout << node->key << "|" << Senses.at(node->key);
    } else {
        std::cout << node->key << "|INVALID_SENSE_KEY";
    }

    // If it has an output node, show it as a leaf
    if (node->outputNode) {
        std::cout << "  -> OUT:" << node->outputNode->key;
    }
    std::cout << "\n";

    // Recurse to children
    for (InputNode* cn : node->inputNodes) {
        printBrain(cn, depth + 1);
    }
}
};
