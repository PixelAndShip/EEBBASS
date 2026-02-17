#pragma once
#include <vector>
#include <memory>
class OutputNode;

class ConditionalInputNode{
public:
float weight;
float setAmount;
unsigned int key;
std::vector<OutputNode*> outputNodes;
ConditionalInputNode* inputOutputNode;


};
