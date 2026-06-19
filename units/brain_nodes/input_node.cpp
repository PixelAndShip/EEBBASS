#include "input_node.h"

InputNode::InputNode() {};

// float weight = 1;
// float setAmount = 0;
// UnitColor unitColor = {255, 255, 255, 255};
// unsigned int key = 255;
// std::vector<InputNode *> inputNodes = {};
// OutputNode *outputNode = nullptr;

InputNode::InputNode(std::mt19937 &gen, InputNode *copyNode)
{
    std::uniform_int_distribution<> mutationDist(-2, 2);
    std::uniform_int_distribution<> keyMutationChance(0, 100);
    weight = std::clamp(copyNode->getWeight() + mutationDist(gen) / 100.0f, 0.01f, 0.99f);

    setAmount = std::clamp(copyNode->getSetAmount() + mutationDist(gen), 0.0f, 255.0f);

    unitColor = copyNode->getUnitColor();
    key = copyNode->getKey();
    int mutatedKeyChance = keyMutationChance(gen);
    if (mutatedKeyChance <= 5)
    {
        int newKey = static_cast<int>(key) + mutationDist(gen);

        key = static_cast<unsigned int>(
            std::clamp(
                newKey,
                0,
                static_cast<int>(getSenses().size()) - 1));
    }
}

InputNode::InputNode(std::mt19937 &gen)
{
    std::uniform_int_distribution<> weightDist(1, 99);
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