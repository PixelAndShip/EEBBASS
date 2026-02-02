#pragma once
#include "BrainNodes/ConditionalInputNodes.h"


class Brain{
public:
// brain structure
std::vector<ConditionalInputNode*> conditionalInputNodes;
// output nodes pointer

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