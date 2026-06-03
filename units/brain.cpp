#include "brain.h"

Brain::Brain() {}

Brain::Brain(float eRadiation, Brain &iBrain)
{
    // initialize brain
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

        inputNodes.insert(inputNodes.begin() + i, startNode);
    }
}

void Brain::addConnection(
    float eRadiation, std::mt19937 &gen,
    InputNode *inputChainLast,
    std::uniform_int_distribution<> &mutationChance,
    int level,
    int childNodeCount = 2,
    int brainDepth = 5)
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

            inputChainLast->insertInputNodeAt(i, newInputNode);
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