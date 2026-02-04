#pragma once

class OutputNode;

class ConditionalInputNode{
public:
float weight;
std::vector<OutputNode*> outputNodes;
std::vector<ConditionalInputNode*> inputOutputNode;
};
