#pragma once
#include <vector>
#include <memory>
class OutputNode;

class ConditionalInputNode{
public:
float weight;
float setAmount;
unsigned int key=255;
std::vector<ConditionalInputNode*> InputNodes;
OutputNode* OutputNode;

~ConditionalInputNode();

};
