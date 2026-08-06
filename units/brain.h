#pragma once

#include <iostream>
#include <sstream>
#include "brain_nodes/input_node.h"

// class OutputNode;
class Brain
{
private:
    int env_identifier;
    std::vector<InputNode *> inputNodes;
    std::unordered_map<std::string, InputNode *> customNodes;

public:
    Brain();

    Brain(int identifier, float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth);

    Brain(int identifier, float eRadiation, std::mt19937 &gen, const Brain &iBrain, int childNodeCount, int brainDepth);

    ~Brain();

    Brain(const Brain &) = delete;
    Brain &operator=(const Brain &) = delete;
    Brain(Brain &&) = delete;
    Brain &operator=(Brain &&) = delete;

    void addCopiedConnection(float eRadiation, std::mt19937 &gen, InputNode *parentLastInputNode, InputNode *lastInputNode, int level, int childNodeCount, int brainDepth);

    const std::vector<InputNode *> &getInputNodes() const;
    void addConnection(float eRadiation, std::mt19937 &gen, InputNode *inputChainLast, std::uniform_int_distribution<> &mutationChance, int level, int childNodeCount, int brainDepth);
    void manageConnections();
    void sendSignal();

    void setIdentifier(int iD) { env_identifier = iD; };
    int getIdentifier() { return env_identifier; };
    void logBrain() const;
    std::string outputBrain(InputNode *node, int depth = 0) const;

    void constructCustomBrain(std::string fileName);
    void constructCustomNode(std::string line);
    InputNode *getCustomParentNode(InputNode *parent, std::vector<int> coordinates, int index);
};