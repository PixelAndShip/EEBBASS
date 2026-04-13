#include "inputNode.h"

InputNode::~InputNode()
{
    for (InputNode *child : inputNodes)
    {
        delete child;
    }
    delete outputNode;
}

void InputNode::setWeight(float iW)
{
    weight = iW;
}
void InputNode::setUnit(char iU)
{
    unit = iU;
}
void InputNode::setSetAmount(float iSA)
{
    setAmount = iSA;
}
void InputNode::setUnitColor(UnitColor iC)
{
    unitColor = iC;
}

void InputNode::setKey(unsigned int iK)
{
    key = iK;
}
void InputNode::appendInputNode(InputNode *iN)
{
    inputNodes.push_back(iN);
}

void InputNode::insertInputNodeAt(int index, InputNode *iN)
{
    if (index >= 0 and index <= static_cast<int>(inputNodes.size()))
    {
        inputNodes.insert(inputNodes.begin() + index, iN);
    }
}

void InputNode::deleteInputNode(int index)
{
    inputNodes.erase(inputNodes.begin() + index);
}

void InputNode::setOutputNode(OutputNode *iON)
{
    outputNode = iON;
}

float InputNode::getWeight()
{
    return weight;
}
char InputNode::getUnit()
{
    return unit;
}
float InputNode::getSetAmount()
{
    return setAmount;
}
UnitColor InputNode::getUnitColor()
{
    return unitColor;
}
unsigned int InputNode::getKey()
{
    return key;
}
const std::vector<InputNode *> &InputNode::getInputNodes() const
{
    return inputNodes;
}
OutputNode *InputNode::getOutputNode()
{
    return outputNode;
}