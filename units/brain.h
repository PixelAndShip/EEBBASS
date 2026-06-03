#pragma once

#include <iostream>

#include "brain_nodes/input_node.h"

// class OutputNode;
class Brain
{
private:
    std::vector<InputNode *> inputNodes;

public:
    Brain();

    Brain(float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth);

    Brain(float eRadiation, Brain &iBrain);

    ~Brain();

    Brain(const Brain &) = delete;
    Brain &operator=(const Brain &) = delete;
    Brain(Brain &&) = delete;
    Brain &operator=(Brain &&) = delete;

    const std::vector<InputNode *> &getInputNodes() const;
    void addConnection(float eRadiation, std::mt19937 &gen, InputNode *inputChainLast, std::uniform_int_distribution<> &mutationChance, int level, int childNodeCount, int brainDepth);
    void manageConnections();
    void sendSignal();
};