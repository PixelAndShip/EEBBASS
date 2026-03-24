#include "ConditionalInputNode.h"

ConditionalInputNode::~ConditionalInputNode(){
    if (OutputNode!=nullptr){
        delete OutputNode;
    }
    if (InputNodes.size()!=0){
        for(int i = 0; i<InputNodes.size()-1;i++){
            delete InputNodes[i];
        }
    }
    
}