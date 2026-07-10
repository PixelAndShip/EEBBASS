#include "brain.h"

Brain::Brain()
{
    DEBUG_LOG("Default Brain constructor called");
}

Brain::Brain(float eRadiation, std::mt19937 &gen, const Brain &iBrain, int childNodeCount, int brainDepth)
{
    DEBUG_LOG("Starting Brain copy constructor");

    std::vector<InputNode *> copyInputNodes = iBrain.getInputNodes();
    DEBUG_LOG("Copying " << copyInputNodes.size() << " root InputNode(s)");

    for (InputNode *iN : copyInputNodes)
    {
        DEBUG_LOG("Copying root InputNode at " << iN);

        InputNode *newInputNode = new InputNode(gen, iN);
        inputNodes.push_back(newInputNode);

        addCopiedConnection(eRadiation, gen, iN, newInputNode, 0, childNodeCount, brainDepth);
    }

    DEBUG_LOG("Finished Brain copy constructor");
}

Brain::~Brain()
{
    DEBUG_LOG("Destroying Brain");

    DEBUG_LOG("Deleting " << inputNodes.size() << " root InputNode(s)");
    for (InputNode *node : inputNodes)
    {
        DEBUG_LOG("Deleting root InputNode at " << node);
        delete node;
    }

    DEBUG_LOG("Finished destroying Brain");
}

Brain::Brain(float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth)
{
    DEBUG_LOG("Starting random Brain constructor");

    int InputNodeCount = dist(gen);
    DEBUG_LOG("Generating " << InputNodeCount << " root InputNode(s)");

    std::uniform_int_distribution<> mutationChance(0, 100);

    for (int i = 0; i < InputNodeCount; i++)
    {
        DEBUG_LOG("Creating root InputNode " << (i + 1) << "/" << InputNodeCount);

        InputNode *startNode = new InputNode(gen);
        inputNodes.push_back(startNode);

        addConnection(eRadiation, gen, startNode, mutationChance, 0, childNodeCount, brainDepth);
    }

    DEBUG_LOG("Finished random Brain constructor");
}

void Brain::addCopiedConnection(
    float eRadiation,
    std::mt19937 &gen,
    InputNode *parentLastInputNode,
    InputNode *lastInputNode,
    int level,
    int childNodeCount,
    int brainDepth)
{
    DEBUG_LOG("Copying connections at level " << level);

    std::uniform_int_distribution<> newNodeChance(0, 100);

    if (parentLastInputNode->getInputNodes().empty())
    {
        DEBUG_LOG("No child InputNodes to copy");
        return;
    }

    for (int i = 0; i < parentLastInputNode->getInputNodes().size(); i++)
    {
        DEBUG_LOG("Copying child InputNode " << (i + 1) << "/" << parentLastInputNode->getInputNodes().size());

        InputNode *copyNode = parentLastInputNode->getInputNodes()[i];
        InputNode *copiedNode = new InputNode(gen, copyNode);

        if (copyNode->getOutputNode() != nullptr)
        {
            DEBUG_LOG("Copying OutputNode");

            OutputNode *copyOutNode = copyNode->getOutputNode();
            OutputNode *copiedOutNode = new OutputNode(gen, copyOutNode);
            copiedNode->setOutputNode(copiedOutNode);
        }

        lastInputNode->appendInputNode(copiedNode);

        if (newNodeChance(gen) <= 5 &&
            parentLastInputNode->getInputNodes().size() < childNodeCount)
        {
            DEBUG_LOG("Mutation: adding additional InputNode");

            InputNode *newNode = new InputNode(gen);
            lastInputNode->appendInputNode(newNode);
        }

        if (newNodeChance(gen) <= 5 &&
            copiedNode->getOutputNode() != nullptr)
        {
            DEBUG_LOG("Mutation: replacing OutputNode");

            OutputNode *newOutputNode = new OutputNode(gen);
            copiedNode->setOutputNode(newOutputNode);
        }

        addCopiedConnection(
            eRadiation,
            gen,
            copyNode,
            copiedNode,
            level + 1,
            childNodeCount,
            brainDepth);
    }

    DEBUG_LOG("Finished copying connections at level " << level);
}

void Brain::addConnection(
    float eRadiation,
    std::mt19937 &gen,
    InputNode *inputChainLast,
    std::uniform_int_distribution<> &mutationChance,
    int level,
    int childNodeCount,
    int brainDepth)
{
    DEBUG_LOG("Generating connections at level " << level);

    if (level >= brainDepth)
    {
        DEBUG_LOG("Reached maximum brain depth");
        return;
    }

    bool mutated = (mutationChance(gen) / 100.0) <= eRadiation;
    DEBUG_LOG("Mutation result: " << (mutated ? "true" : "false"));

    if (mutated)
    {
        std::uniform_int_distribution<> dist(0, childNodeCount);

        int nextNodesCount = dist(gen);
        DEBUG_LOG("Creating " << nextNodesCount << " child InputNode(s)");

        for (int i = 0; i < nextNodesCount; i++)
        {
            DEBUG_LOG("Creating child InputNode " << (i + 1)
                                                  << "/" << nextNodesCount);

            InputNode *newInputNode = new InputNode(gen);

            inputChainLast->appendInputNode(newInputNode);

            addConnection(
                eRadiation,
                gen,
                newInputNode,
                mutationChance,
                level + 1,
                childNodeCount,
                brainDepth);
        }
    }
    else if (inputChainLast->getKey() != 255)
    {
        DEBUG_LOG("Reached terminal InputNode");

        if (inputChainLast->getOutputNode() == nullptr)
        {
            DEBUG_LOG("Creating OutputNode");

            OutputNode *newOutputNode = new OutputNode(gen);

            inputChainLast->setOutputNode(newOutputNode);
        }
        else
        {
            DEBUG_LOG("OutputNode already exists");
        }

        // loop through brain and check if node already exists,
        // 0.5 chance to connect to it or make another one
    }

    DEBUG_LOG("Finished generating connections at level " << level);
}

const std::vector<InputNode *> &Brain::getInputNodes() const
{
    DEBUG_LOG("Getting Brain root InputNodes (count = "
              << inputNodes.size() << ")");

    return inputNodes;
}