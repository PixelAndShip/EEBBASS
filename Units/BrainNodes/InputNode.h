#pragma once
#include <vector>
#include <memory>
#include "OutputNode.h"
#include <any>


class InputNode{
public:
float weight;
char something;
float setAmount;
int red,green,blue,transparency;
unsigned int key=255;
std::vector<InputNode*> inputNodes;
OutputNode* outputNode;





};
