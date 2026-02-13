#pragma once
#include <vector>
class OutputNode;

class ConditionalInputNode{
public:
float weight;
float setAmount;
std::vector<OutputNode*> outputNodes;
ConditionalInputNode* inputOutputNode;
};
