#include "brain.h"
#include "BrainNodes/ConditionalInputNode.h"
#include "BrainNodes/OutputNode.h"


Brain::Brain(){}


Brain::Brain(float eRadiation, Brain iBrain){
    // initialize brain
}

Brain::~Brain(){
    for (ConditionalInputNode* inputNode : conditionalInputNodes){
        deleteChain(inputNode);
    }
}

void Brain::deleteChain(ConditionalInputNode* node){
    if (!node){
        return;
    }
    deleteChain(node->inputOutputNode);

    for (OutputNode* outputNode : node->outputNodes){
        delete outputNode;
    }

    delete node;
}

Brain::Brain(float eRadiation, std::mt19937& gen, std::uniform_int_distribution<>& dist){
    
    int conditionalInputNodeCount = dist(gen);
    conditionalInputNodes.resize(conditionalInputNodeCount);
    std::uniform_int_distribution<> sensesDist(0,Senses.size()-1);
    

    std::uniform_int_distribution<> actionsDist(0,Actions.size()-1);
    

    std::uniform_int_distribution<> mutationChance(0,100);


    for(int i = 0;i<conditionalInputNodeCount;i++){
        ConditionalInputNode* startNode = new ConditionalInputNode();
        addConnection(eRadiation,false,gen,startNode,sensesDist,actionsDist,mutationChance);
        conditionalInputNodes[i] = startNode;
        
    }
    
}


void Brain::addConnection(float eRadiation,bool endOfChain, std::mt19937& gen,ConditionalInputNode* inputChainLast, std::uniform_int_distribution<>& sensesDist, std::uniform_int_distribution<>& actionsDist, std::uniform_int_distribution<>& mutationChance){
    if (inputChainLast==nullptr){
        return;
    }

    
    bool mutated = (mutationChance(gen)/100.0)<=eRadiation;
    
    if (mutated and endOfChain){
        size_t mutatedAction = actionsDist(gen);
        auto node = std::next(Actions.begin(),mutatedAction);
        OutputNode* newOutputNode = new OutputNode();
        newOutputNode->key = node->first;
        // loop through brain and check if node already exists, 0.5 chance to connect to it or make another one
        mutated = (mutationChance(gen)/100.0)<=eRadiation;
        
        if(mutated){
            addConnection(eRadiation,endOfChain,gen,inputChainLast,sensesDist,actionsDist,mutationChance);
        }
    }
    if(mutated and !endOfChain){
        size_t mutatedSense = sensesDist(gen);
        auto node = std::next(Senses.begin(),mutatedSense);
        ConditionalInputNode* newInputNode = new ConditionalInputNode();
        newInputNode->key = node->first;
        // loop through brain and check if node already exists, 0.5 chance to connect to it or make another one
        mutated = (mutationChance(gen)/100.0)<=eRadiation;
        endOfChain = (mutationChance(gen)/100.0)<=eRadiation;
       
        if(mutated){
            // need to check if newInputNode or inputChainLast->inputOutputNode needs to be parsed
            addConnection(eRadiation,endOfChain,gen,newInputNode,sensesDist,actionsDist,mutationChance);
        }

    }
    
    
    return;
}
