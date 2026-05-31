#include "input_node.h"

InputNode::InputNode() {};

// float weight = 1;
// float setAmount = 0;
// UnitColor unitColor = {255, 255, 255, 255};
// unsigned int key = 255;
// std::vector<InputNode *> inputNodes = {};
// OutputNode *outputNode = nullptr;

InputNode::InputNode(std::mt19937 &gen)
{
    std::uniform_int_distribution<> weightDist(0, 99);
    std::uniform_int_distribution<> setAmountDist(0, 255);
    std::uniform_int_distribution<> unitColorRedDist(0, 255);
    std::uniform_int_distribution<> unitColorGreenDist(0, 255);
    std::uniform_int_distribution<> unitColorBlueDist(0, 255);
    std::uniform_int_distribution<> unitColorTransparencyDist(0, 255);
    std::uniform_int_distribution<> sensesDist(0, getSenses().size() - 1);
    weight = weightDist(gen) / 100.0;
    setAmount = setAmountDist(gen);
    unitColor = {
        (unsigned int)unitColorRedDist(gen),
        (unsigned int)unitColorGreenDist(gen),
        (unsigned int)unitColorBlueDist(gen),
        (unsigned int)unitColorTransparencyDist(gen)};
    key = sensesDist(gen);
}

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