#pragma once
#include "BrainNodes/ConditionalInputNode.h"
#include "BrainNodes/NodeVariables.cpp"
#include <random>
class Brain{
public:
// brain structure
std::vector<ConditionalInputNode*> conditionalInputNodes;
std::vector<int> actions;

Brain(){}

Brain(float eRadiation, std::mt19937& gen, std::uniform_int_distribution<>& dist){
    // initialize brain
    int conditionalInputNodeCount = dist(gen);
    for (int i = 0; i<conditionalInputNodeCount;i++){
        
    }
}

Brain(float eRadiation, Brain iBrain){
    // initialize brain
}


void manageConnections(){

}


void sendSignal(){

}
};