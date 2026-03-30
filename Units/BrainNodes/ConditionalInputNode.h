#pragma once
#include <vector>
#include <memory>
#include "OutputNode.h"

class ConditionalInputNode{
public:
float weight;
float setAmount;
unsigned int key=255;
std::vector<ConditionalInputNode*> inputNodes;
OutputNode* outputNode;



};
