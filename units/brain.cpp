#include "brain.h"

Brain::Brain() {}

Brain::Brain(float eRadiation, std::mt19937 &gen, const Brain &iBrain, int childNodeCount, int brainDepth)
{
    // initialize brain
    std::vector<InputNode *> copyInputNodes = iBrain.getInputNodes();

    for (InputNode *iN : copyInputNodes)
    {
        InputNode *newInputNode = new InputNode(gen, iN);
        addCopiedConnection(eRadiation, gen, iN, newInputNode, 0, childNodeCount, brainDepth);
    }
}

Brain::~Brain()
{
    for (InputNode *node : inputNodes)
    {
        delete node;
    }
}

Brain::Brain(float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth)
{

    int InputNodeCount = dist(gen);

    std::uniform_int_distribution<> mutationChance(0, 100);

    for (int i = 0; i < InputNodeCount; i++)
    {

        InputNode *startNode = new InputNode(gen);
        addConnection(eRadiation, gen, startNode, mutationChance, 0, childNodeCount, brainDepth);

        inputNodes.push_back(startNode);
    }
}

void Brain::addCopiedConnection(
    float eRadiation, std::mt19937 &gen, InputNode *parentLastInputNode, InputNode *lastInputNode, int level, int childNodeCount = 3, int brainDepth = 4)
{
    inputNodes.push_back(lastInputNode);
    std::uniform_int_distribution<> newNodeChance(0, 100);
    if (parentLastInputNode->getInputNodes().empty())
    {
        return;
    }
    for (int i = 0; i < parentLastInputNode->getInputNodes().size(); i++)
    {

        InputNode *copyNode = parentLastInputNode->getInputNodes()[i];
        InputNode *copiedNode = new InputNode(gen, copyNode);
        if (copyNode->getOutputNode() != nullptr)
        {
            OutputNode *copyOutNode = copyNode->getOutputNode();
            OutputNode *copiedOutNode = new OutputNode(gen, copyOutNode);
            copiedNode->setOutputNode(copiedOutNode);
        }
        lastInputNode->appendInputNode(copiedNode);
        if (newNodeChance(gen) <= 5 and parentLastInputNode->getInputNodes().size() < childNodeCount)
        {
            InputNode *newNode = new InputNode(gen);
            lastInputNode->appendInputNode(newNode);
        }
        if (newNodeChance(gen) <= 5 and copiedNode->getOutputNode() != nullptr)
        {
            OutputNode *newOutputNode = new OutputNode(gen);
            copiedNode->setOutputNode(newOutputNode);
        }
        addCopiedConnection(eRadiation, gen, copyNode, copiedNode, level + 1, childNodeCount, brainDepth);
    }
    // need to add action child node processing
}

void Brain::addConnection(
    float eRadiation, std::mt19937 &gen,
    InputNode *inputChainLast,
    std::uniform_int_distribution<> &mutationChance,
    int level,
    int childNodeCount = 3,
    int brainDepth = 4)
{

    if (level >= brainDepth)
    {
        return;
    }

    bool mutated = (mutationChance(gen) / 100.0) <= eRadiation;

    if (mutated)
    {

        std::uniform_int_distribution<> dist(0, childNodeCount); // adjust child node count
        int nextNodesCount = dist(gen);

        for (int i = 0; i < nextNodesCount; i++)
        {

            InputNode *newInputNode = new InputNode(gen); // initalize attributes

            inputChainLast->appendInputNode(newInputNode);
            addConnection(eRadiation, gen, newInputNode, mutationChance, level + 1, childNodeCount, brainDepth);
        }
    }
    else if (inputChainLast->getKey() != 255)
    {
        if (inputChainLast->getOutputNode() == nullptr)
        {

            OutputNode *newOutputNode = new OutputNode(gen); // initalize attributes

            inputChainLast->setOutputNode(newOutputNode);
        }

        // loop through brain and check if node already exists, 0.5 chance to connect to it or make another one
    }

    return;
}

const std::vector<InputNode *> &Brain::getInputNodes() const
{
    return inputNodes;
}