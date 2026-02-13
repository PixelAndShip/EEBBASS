#pragma once


#include "BrainNodes/NodeVariables.cpp"
#include <random>
#include <iterator>

class ConditionalInputNode;
class OutputNode;
class Brain{
public:
// brain structure
std::vector<ConditionalInputNode*> conditionalInputNodes;
std::vector<int> actions;

Brain();

Brain(float eRadiation, std::mt19937& gen, std::uniform_int_distribution<>& dist);

Brain(float eRadiation, Brain iBrain);

void addConnection(float eRadiation,bool endOfChain, std::mt19937& gen,ConditionalInputNode* inputChainLast, std::uniform_int_distribution<>& sensesDist, std::uniform_int_distribution<>& actionsDist, std::uniform_int_distribution<>& mutationChance);


void manageConnections();

void sendSignal();
};