#pragma once
#include <vector>
class OutputNode;

class ConditionalInputNode{
public:
float weight;
float setAmount;
unsigned int key;
std::vector<OutputNode*> outputNodes;
ConditionalInputNode* inputOutputNode;
};
