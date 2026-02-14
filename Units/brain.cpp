#include "brain.h"
#include "BrainNodes/ConditionalInputNode.h"
#include "BrainNodes/OutputNode.h"


Brain::Brain(){}


Brain::Brain(float eRadiation, Brain iBrain){
    // initialize brain
}

Brain::Brain(float eRadiation, std::mt19937& gen, std::uniform_int_distribution<>& dist){
    // initialize brain
    int conditionalInputNodeCount = dist(gen);
    
    ConditionalInputNode* inputChainFirst;

    std::uniform_int_distribution<> sensesDist(0,Senses.size()-1);
    

    std::uniform_int_distribution<> actionsDist(0,Actions.size()-1);
    

    std::uniform_int_distribution<> mutationChance(0,100);


    for(int i = 0;i<conditionalInputNodeCount;i++){
        ConditionalInputNode* startNode = new ConditionalInputNode();
        conditionalInputNodes[i] = startNode;
        addConnection(eRadiation,false,gen,startNode,sensesDist,actionsDist,mutationChance);
        conditionalInputNodes[i] = startNode->inputOutputNode;
    }
}


void Brain::addConnection(float eRadiation,bool endOfChain, std::mt19937& gen,ConditionalInputNode* inputChainLast, std::uniform_int_distribution<>& sensesDist, std::uniform_int_distribution<>& actionsDist, std::uniform_int_distribution<>& mutationChance){
    if (inputChainLast==nullptr){
        return;
    }

    
    bool mutated = (mutationChance(gen)/100.0)<=eRadiation;
    
    if (mutated and endOfChain){
        size_t mutatedAction = actionsDist(gen);
        auto nodeName = std::next(Actions.begin(),mutatedAction);
        OutputNode* newOutputNode = new OutputNode();
        // initialize action
        mutated = (mutationChance(gen)/100.0)<=eRadiation;
        if(mutated){
            addConnection(eRadiation,endOfChain,gen,inputChainLast,sensesDist,actionsDist,mutationChance);
        }
    }
    if(mutated and !endOfChain){
        size_t mutatedSense = sensesDist(gen);
        auto nodeName = std::next(Senses.begin(),mutatedSense);
        ConditionalInputNode* newInputNode = new ConditionalInputNode();
        // initialize sense
        mutated = (mutationChance(gen)/100.0)<=eRadiation;
        endOfChain = (mutationChance(gen)/100.0)<=eRadiation;
        if(mutated){
            addConnection(eRadiation,endOfChain,gen,newInputNode,sensesDist,actionsDist,mutationChance);
        }

    }
    
    
    return;
}
