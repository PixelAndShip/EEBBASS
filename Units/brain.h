#pragma once
#include "BrainNodes/ConditionalInputNodes.h"
#include "BrainNodes/NodeVariables.cpp"

class Brain{
public:
// brain structure
std::vector<ConditionalInputNode*> conditionalInputNodes;
std::vector<int> actions;

Brain(){}

Brain(float eRadiation){
    // initialize brain
}

Brain(float eRadiation, Brain iBrain){
    // initialize brain
}


void manageConnections(){

}


void sendSignal(){

}
};