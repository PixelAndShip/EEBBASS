#include "brain.h"

Brain::Brain()
{
}

Brain::Brain(int identifier, float eRadiation, std::mt19937 &gen, const Brain &iBrain, int childNodeCount, int brainDepth)
{

    env_identifier = identifier;

    std::vector<InputNode *> copyInputNodes = iBrain.getInputNodes();

    for (InputNode *iN : copyInputNodes)
    {

        InputNode *newInputNode = new InputNode(gen, iN);
        inputNodes.push_back(newInputNode);

        addCopiedConnection(eRadiation, gen, iN, newInputNode, 0, childNodeCount, brainDepth);
    }
    logBrain();
    logBrain();
}

Brain::~Brain()
{

    for (InputNode *node : inputNodes)
    {

        delete node;
    }
}

Brain::Brain(int identifier, float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth)
{

    env_identifier = identifier;

    int InputNodeCount = dist(gen);

    std::uniform_int_distribution<> mutationChance(0, 100);

    for (int i = 0; i < InputNodeCount; i++)
    {

        InputNode *startNode = new InputNode(gen);
        inputNodes.push_back(startNode);

        addConnection(eRadiation, gen, startNode, mutationChance, 0, childNodeCount, brainDepth);
    }
    logBrain();
    logBrain();
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

    std::uniform_int_distribution<> newNodeChance(0, 100);

    if (parentLastInputNode->getOutputNode() != nullptr)
    {
        OutputNode *copyOutNode =
            parentLastInputNode->getOutputNode();

        OutputNode *copiedOutNode =
            new OutputNode(gen, copyOutNode);

        lastInputNode->setOutputNode(copiedOutNode);
    }

    for (InputNode *copyNode :
         parentLastInputNode->getInputNodes())
    {
        InputNode *copiedNode =
            new InputNode(gen, copyNode);

        lastInputNode->appendInputNode(copiedNode);

        if (newNodeChance(gen) <= 5 and
            copiedNode->getOutputNode() != nullptr)
        {
            OutputNode *newOutputNode =
                new OutputNode(gen);

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

    if (newNodeChance(gen) <= 5 and
        lastInputNode->getInputNodes().size() < childNodeCount and
        level + 1 < brainDepth)
    {

        InputNode *newNode =
            new InputNode(gen);

        lastInputNode->appendInputNode(newNode);

        addConnection(
            eRadiation,
            gen,
            newNode,
            newNodeChance,
            level + 1,
            childNodeCount,
            brainDepth);
    }
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

    if (level >= brainDepth)
    {

        return;
    }

    bool mutated = (mutationChance(gen) / 100.0) <= eRadiation;

    if (mutated)
    {
        std::uniform_int_distribution<> dist(0, childNodeCount);

        int nextNodesCount = dist(gen);

        for (int i = 0; i < nextNodesCount; i++)
        {

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

        if (inputChainLast->getOutputNode() == nullptr)
        {

            OutputNode *newOutputNode = new OutputNode(gen);

            inputChainLast->setOutputNode(newOutputNode);
        }
        else
        {
        }

        // loop through brain and check if node already exists,
        // 0.5 chance to connect to it or make another one
    }
}

const std::vector<InputNode *> &Brain::getInputNodes() const
{

    return inputNodes;
}

void Brain::logBrain()
{

    std::stringstream writtenData;
    std::string fileName = "logs/Agent_Brain_Log_" + std::to_string(env_identifier) + ".txt";
    std::ifstream CurrentLog(fileName);
    if (CurrentLog)
    {
        writtenData << CurrentLog.rdbuf();
    }
    std::string data = writtenData.str() + "\n";

    for (InputNode *iN : inputNodes)
    {

        data += outputBrain(iN, 0);
    }

    CurrentLog.close();

    std::ofstream updatedLog(fileName);

    if (data == "")
    {
        data = "No brain found!";
    }
    updatedLog << data;

    updatedLog.close();
}

std::string Brain::outputBrain(InputNode *node, int depth)
{

    if (!node)
    {
        return "";
    }

    std::string data = "";

    std::string indent(
        depth * 2,
        ' ');

    data += indent;

    unsigned int key = node->getKey();

    if (key != 255)
    {

        data +=
            std::to_string(key) + "|" + getSenses().at(key) + "|" + std::to_string(node->getWeight());
    }
    else
    {

        data +=
            std::to_string(key) + "|INVALID_SENSE_KEY";
    }

    if (node->getOutputNode())
    {

        unsigned int outputKey =
            node->getOutputNode()->getKey();

        if (outputKey < getActions().size())
        {
            data +=
                "---" + std::to_string(outputKey) + "|" + getActions().at(outputKey) + "|" + std::to_string(node->getOutputNode()->getWeight());
        }
        else
        {

            data +=
                "---" + std::to_string(outputKey) + "|INVALID_ACTION_KEY";
        }
    }

    data += "\n";

    for (InputNode *cn :
         node->getInputNodes())
    {
        data += outputBrain(
            cn,
            depth + 1);
    }

    return data;
}
