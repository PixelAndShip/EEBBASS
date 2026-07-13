#include "input_node.h"

InputNode::InputNode()
{
    DEBUG_LOG("Default InputNode constructor called");
}

// float weight = 1;
// float setAmount = 0;
// UnitColor unitColor = {255, 255, 255, 255};
// unsigned int key = 255;
// std::vector<InputNode *> inputNodes = {};
// OutputNode *outputNode = nullptr;

InputNode::InputNode(std::mt19937 &gen, InputNode *copyNode)
{
    DEBUG_LOG("Starting InputNode copy constructor");

    std::uniform_int_distribution<> mutationDist(-2, 2);
    std::uniform_int_distribution<> keyMutationChance(0, 100);

    weight = std::clamp(copyNode->getWeight() + mutationDist(gen) / 100.0f, 0.01f, 0.99f);
    DEBUG_LOG("Copied and mutated weight to: " << weight);

    setAmount = std::clamp(copyNode->getSetAmount() + mutationDist(gen), 0.0f, 255.0f);
    DEBUG_LOG("Copied and mutated setAmount to: " << setAmount);

    unitColor = copyNode->getUnitColor();
    DEBUG_LOG("Copied color: "
              << unitColor.red << ", "
              << unitColor.green << ", "
              << unitColor.blue << ", "
              << unitColor.transparency);

    key = copyNode->getKey();
    DEBUG_LOG("Copied key: " << key);

    int mutatedKeyChance = keyMutationChance(gen);
    DEBUG_LOG("Key mutation roll: " << mutatedKeyChance);

    if (mutatedKeyChance <= 5)
    {
        int newKey = static_cast<int>(key) + mutationDist(gen);

        key = static_cast<unsigned int>(
            std::clamp(
                newKey,
                0,
                static_cast<int>(getSenses().size()) - 1));

        DEBUG_LOG("Key mutated to: " << key);
    }

    DEBUG_LOG("Finished InputNode copy constructor");
}

InputNode::InputNode(std::mt19937 &gen)
{
    DEBUG_LOG("Starting random InputNode constructor");

    std::uniform_int_distribution<> weightDist(1, 99);
    std::uniform_int_distribution<> setAmountDist(0, 255);
    std::uniform_int_distribution<> unitColorRedDist(0, 255);
    std::uniform_int_distribution<> unitColorGreenDist(0, 255);
    std::uniform_int_distribution<> unitColorBlueDist(0, 255);
    std::uniform_int_distribution<> unitColorTransparencyDist(0, 255);
    std::uniform_int_distribution<> sensesDist(0, getSenses().size() - 1);

    weight = weightDist(gen) / 100.0;
    DEBUG_LOG("Generated weight: " << weight);

    setAmount = setAmountDist(gen);
    DEBUG_LOG("Generated setAmount: " << setAmount);

    unitColor = {
        (unsigned int)unitColorRedDist(gen),
        (unsigned int)unitColorGreenDist(gen),
        (unsigned int)unitColorBlueDist(gen),
        (unsigned int)unitColorTransparencyDist(gen)};

    DEBUG_LOG("Generated color: "
              << unitColor.red << ", "
              << unitColor.green << ", "
              << unitColor.blue << ", "
              << unitColor.transparency);

    key = sensesDist(gen);
    DEBUG_LOG("Generated key: " << key);

    DEBUG_LOG("Finished random InputNode constructor");
}

InputNode::~InputNode()
{
    DEBUG_LOG("Destroying InputNode");

    DEBUG_LOG("Deleting " << inputNodes.size() << " child InputNode(s)");
    for (InputNode *child : inputNodes)
    {
        DEBUG_LOG("Deleting child InputNode at " << child);
        delete child;
    }

    DEBUG_LOG("Deleting OutputNode at " << outputNode);
    delete outputNode;

    DEBUG_LOG("Finished destroying InputNode");
}

void InputNode::setWeight(float iW)
{
    DEBUG_LOG("Setting weight from " << weight << " to " << iW);
    weight = iW;
}

void InputNode::setSetAmount(float iSA)
{
    DEBUG_LOG("Setting setAmount from " << setAmount << " to " << iSA);
    setAmount = iSA;
}

void InputNode::setUnitColor(UnitColor iC)
{
    DEBUG_LOG("Setting color from ("
              << unitColor.red << ", "
              << unitColor.green << ", "
              << unitColor.blue << ", "
              << unitColor.transparency << ") to ("
              << iC.red << ", "
              << iC.green << ", "
              << iC.blue << ", "
              << iC.transparency << ")");

    unitColor = iC;
}

void InputNode::setKey(unsigned int iK)
{
    DEBUG_LOG("Setting InputNode key from " << key << " to " << iK);
    key = iK;
}

void InputNode::appendInputNode(InputNode *iN)
{
    DEBUG_LOG("Appending child InputNode at " << iN);
    inputNodes.push_back(iN);
}

void InputNode::insertInputNodeAt(int index, InputNode *iN)
{
    if (index >= 0 && index <= static_cast<int>(inputNodes.size()))
    {
        DEBUG_LOG("Inserting child InputNode at index " << index
                                                        << " (pointer " << iN << ")");
        inputNodes.insert(inputNodes.begin() + index, iN);
    }
    else
    {
        DEBUG_LOG("Failed to insert InputNode: invalid index "
                  << index << ", size = " << inputNodes.size());
    }
}

void InputNode::deleteInputNode(int index)
{
    if (index >= 0 && index < static_cast<int>(inputNodes.size()))
    {
        DEBUG_LOG("Removing child InputNode at index "
                  << index << " (pointer "
                  << inputNodes[index] << ")");
        inputNodes.erase(inputNodes.begin() + index);
    }
    else
    {
        DEBUG_LOG("Failed to remove InputNode: invalid index "
                  << index << ", size = " << inputNodes.size());
    }
}

void InputNode::setOutputNode(OutputNode *iON)
{
    DEBUG_LOG("Setting OutputNode from " << outputNode << " to " << iON);
    outputNode = iON;
}

float InputNode::getWeight()
{
    DEBUG_LOG("Getting InputNode weight: " << weight);
    return weight;
}

float InputNode::getSetAmount()
{
    DEBUG_LOG("Getting InputNode setAmount: " << setAmount);
    return setAmount;
}

UnitColor InputNode::getUnitColor()
{
    DEBUG_LOG("Getting InputNode color");
    return unitColor;
}

unsigned int InputNode::getKey()
{
    DEBUG_LOG("Getting InputNode key: " << key);
    return key;
}

const std::vector<InputNode *> &InputNode::getInputNodes() const
{
    DEBUG_LOG("Getting InputNode children (count = " << inputNodes.size() << ")");
    return inputNodes;
}

OutputNode *InputNode::getOutputNode()
{
    DEBUG_LOG("Getting OutputNode pointer: " << outputNode);
    return outputNode;
}