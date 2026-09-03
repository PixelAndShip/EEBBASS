#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "brain_nodes/input_node.h"

class Brain
{
private:
    int env_identifier;
    std::vector<InputNode *> inputNodes;
    std::unordered_map<std::string, InputNode *> customNodes;

public:
    Brain();

    Brain(std::string data, int iE);

    Brain(int identifier, float eRadiation, int childNodeCount, int brainDepth);

    Brain(int identifier, float eRadiation, const Brain &iBrain, int maxRootNodesCount, int childNodeCount, int brainDepth);

    ~Brain();

    Brain(const Brain &) = delete;
    Brain &operator=(const Brain &) = delete;
    Brain(Brain &&) = delete;
    Brain &operator=(Brain &&) = delete;

    void addCopiedConnection(float eRadiation, InputNode *parentLastInputNode, InputNode *lastInputNode, int level, int childNodeCount, int brainDepth);

    const std::vector<InputNode *> &getInputNodes() const;
    void addConnection(float eRadiation, InputNode *inputChainLast, int level, int childNodeCount, int brainDepth);
    void manageConnections();
    void sendSignal();

    void setIdentifier(int iD) { env_identifier = iD; };
    int getIdentifier() { return env_identifier; };
    std::string logBrain() const;
    std::string outputBrain(InputNode *node, std::vector<int> id) const;

    void constructCustomBrain(std::string fileName);
    void constructCustomNode(std::string line);
};