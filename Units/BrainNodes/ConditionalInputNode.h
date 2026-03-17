#pragma once
#include <vector>
#include <memory>
class OutputNode;

class ConditionalInputNode{
public:
float weight;
float setAmount;
unsigned int key;
std::vector<ConditionalInputNode*> InputNodes;
std::vector<OutputNode*> OutputNodes;


};
