#pragma once

#include <random>
#include <iterator>
#include <cmath>
#include <iostream>

#include "BrainNodes/inputNode.h"

class OutputNode;
class Brain
{
private:
    std::vector<InputNode *> inputNodes;

public:
    Brain();

    Brain(float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist);

    Brain(float eRadiation, Brain &iBrain);

    ~Brain();

    Brain(const Brain &) = delete;
    Brain &operator=(const Brain &) = delete;
    Brain(Brain &&) = delete;
    Brain &operator=(Brain &&) = delete;

    const std::vector<InputNode *> &getInputNodes() const;
    void addConnection(float eRadiation, std::mt19937 &gen, InputNode *inputChainLast, std::uniform_int_distribution<> &sensesDist, std::uniform_int_distribution<> &actionsDist, std::uniform_int_distribution<> &mutationChance, int level);

    void manageConnections();

    void sendSignal();
};