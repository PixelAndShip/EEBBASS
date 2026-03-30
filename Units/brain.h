#pragma once
#include "BrainNodes/NodeVariables.cpp"
#include <random>
#include <iterator>
#include <cmath>
#include <iostream>
#include "BrainNodes/ConditionalInputNode.h"





class OutputNode;
class Brain{
public:
// brain structure
std::vector<ConditionalInputNode*> conditionalInputNodes;


Brain();

Brain(float eRadiation, std::mt19937& gen, std::uniform_int_distribution<>& dist);

Brain(float eRadiation, Brain& iBrain);

~Brain();


Brain(const Brain&) = delete;
Brain& operator=(const Brain&) = delete;
Brain(Brain&&) = delete;
Brain& operator=(Brain&&) = delete;

void addConnection(float eRadiation,std::mt19937& gen,ConditionalInputNode* inputChainLast, std::uniform_int_distribution<>& sensesDist, std::uniform_int_distribution<>& actionsDist, std::uniform_int_distribution<>& mutationChance);


void manageConnections();

void sendSignal();

private:

    static void deleteBrain(ConditionalInputNode* node);


};