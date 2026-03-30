#include "brain.h"
#include "BrainNodes/ConditionalInputNode.h"
#include "BrainNodes/OutputNode.h"



Brain::Brain(){}


Brain::Brain(float eRadiation, Brain& iBrain){
    // initialize brain
}

Brain::~Brain(){
    for (ConditionalInputNode* inputNode : conditionalInputNodes){
        deleteBrain(inputNode);
    }
}
void Brain::deleteBrain(ConditionalInputNode* node){
    if(!node) return;
    for(ConditionalInputNode* child : node->inputNodes){
        deleteBrain(child);
    }
    delete node->outputNode;
    node->outputNode=nullptr;
    delete node;
}

Brain::Brain(float eRadiation,std::mt19937& gen, std::uniform_int_distribution<>& dist){
    
    int conditionalInputNodeCount = 10;//dist(gen);

    conditionalInputNodes.resize(conditionalInputNodeCount);
    std::uniform_int_distribution<> sensesDist(0,Senses.size()-1);
    

    std::uniform_int_distribution<> actionsDist(0,Actions.size()-1);
    

    std::uniform_int_distribution<> mutationChance(0,100);


    for(int i = 0;i<conditionalInputNodeCount;i++){
      
        ConditionalInputNode* startNode = new ConditionalInputNode();
        
        addConnection(eRadiation,gen,startNode,sensesDist,actionsDist,mutationChance);
        
        conditionalInputNodes[i] = startNode;
        
    }
    
}


void Brain::addConnection(float eRadiation,std::mt19937& gen, ConditionalInputNode* inputChainLast, std::uniform_int_distribution<>& sensesDist, std::uniform_int_distribution<>& actionsDist, std::uniform_int_distribution<>& mutationChance){

    

    
    bool mutated = (mutationChance(gen)/100.0)<=eRadiation;
    
    if (mutated){
        
        size_t mutatedSense = sensesDist(gen);
        auto node = std::next(Senses.begin(),mutatedSense);
        inputChainLast->key = node->first;

        std::uniform_int_distribution<> dist(0,2);
        int nextNodesCount = dist(gen);
        inputChainLast->inputNodes.resize(nextNodesCount);

        for(int i = 0;i<inputChainLast->inputNodes.size();i++){  
            ConditionalInputNode* newInputNode = new ConditionalInputNode(); // initalize attributes
            inputChainLast->inputNodes[i]=newInputNode;
            addConnection(eRadiation,gen,newInputNode,sensesDist,actionsDist,mutationChance);
        }
        
        
    }
    else if(inputChainLast->key!=255){
        if(inputChainLast->outputNode==nullptr){
           
            size_t mutatedAction = actionsDist(gen);
            auto node = std::next(Actions.begin(),mutatedAction);
            OutputNode* newOutputNode = new OutputNode(); // initalize attributes
            newOutputNode->key = node->first;
            inputChainLast->outputNode=newOutputNode;

        }
        
        
        // loop through brain and check if node already exists, 0.5 chance to connect to it or make another one
    }
    
    
    return;
}

