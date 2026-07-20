#include "brain.h"

Brain::Brain()
{
    DEBUG_LOG("Default Brain constructor called");
}

Brain::Brain(int identifier, float eRadiation, std::mt19937 &gen, const Brain &iBrain, int childNodeCount, int brainDepth)
{
    DEBUG_LOG("Starting Brain copy constructor");

    env_identifier = identifier;
    DEBUG_LOG("Split identifier from " << identifier << " to " << env_identifier);

    std::vector<InputNode *> copyInputNodes = iBrain.getInputNodes();
    DEBUG_LOG("Copying " << copyInputNodes.size() << " root InputNode(s)");

    for (InputNode *iN : copyInputNodes)
    {
        DEBUG_LOG("Copying root InputNode at " << iN);

        InputNode *newInputNode = new InputNode(gen, iN);
        inputNodes.push_back(newInputNode);

        addCopiedConnection(eRadiation, gen, iN, newInputNode, 0, childNodeCount, brainDepth);
    }
    logBrain();
    logBrain();
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

Brain::Brain(int identifier, float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth)
{
    DEBUG_LOG("Starting random Brain constructor");

    env_identifier = identifier;
    DEBUG_LOG("Identifier from " << identifier << " to " << env_identifier);

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
    logBrain();
    logBrain();
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
        DEBUG_LOG("Mutation: creating new InputNode during copy");

        InputNode *newNode =
            new InputNode(gen);

        lastInputNode->appendInputNode(newNode);

        DEBUG_LOG(
            "New InputNode created at depth "
            << level + 1);

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

void Brain::logBrain()
{
    DEBUG_LOG("Starting brain log");

    std::stringstream writtenData;
    std::string fileName = "logs/Agent_Brain_Log_" + std::to_string(env_identifier) + ".txt";
    std::ifstream CurrentLog(fileName);
    if (CurrentLog)
    {
        writtenData << CurrentLog.rdbuf();
    }
    std::string data = writtenData.str() + "\n";
    DEBUG_LOG("Read log file " + std::to_string(env_identifier));
    for (InputNode *iN : inputNodes)
    {
        DEBUG_LOG("Starting input node log");
        data += outputBrain(iN, 0);
    }

    CurrentLog.close();

    std::ofstream updatedLog(fileName);
    DEBUG_LOG("Writing data to file");
    if (data == "")
    {
        data = "No brain found!";
    }
    updatedLog << data;

    updatedLog.close();

    DEBUG_LOG("Finished simulation cycle logging");
}

std::string Brain::outputBrain(InputNode *node, int depth)
{
    DEBUG_LOG("Outputting brain node at depth "
              << depth
              << " pointer "
              << node);

    if (!node)
    {
        DEBUG_LOG("Node is null");

        return "";
    }

    std::string data = "";

    std::string indent(
        depth * 2,
        ' ');

    data += indent;

    unsigned int key = node->getKey();

    DEBUG_LOG("Node key: "
              << key);

    if (key != 255)
    {
        DEBUG_LOG("Valid sense key");

        data +=
            std::to_string(key) + "|" + getSenses().at(key) + "|" + std::to_string(node->getWeight());
    }
    else
    {
        DEBUG_LOG("Invalid sense key");

        data +=
            std::to_string(key) + "|INVALID_SENSE_KEY";
    }

    if (node->getOutputNode())
    {
        DEBUG_LOG("Node has OutputNode");

        unsigned int outputKey =
            node->getOutputNode()->getKey();

        DEBUG_LOG("Output key: "
                  << outputKey);

        if (outputKey < getActions().size())
        {
            data +=
                "---" + std::to_string(outputKey) + "|" + getActions().at(outputKey) + "|" + std::to_string(node->getOutputNode()->getWeight());
        }
        else
        {
            DEBUG_LOG("Invalid output key");

            data +=
                "---" + std::to_string(outputKey) + "|INVALID_ACTION_KEY";
        }
    }

    data += "\n";

    DEBUG_LOG("Processing "
              << node->getInputNodes().size()
              << " child nodes");

    for (InputNode *cn :
         node->getInputNodes())
    {
        data += outputBrain(
            cn,
            depth + 1);
    }

    DEBUG_LOG("Finished outputting node at depth "
              << depth);

    return data;
}

void Brain::logBrain()
{
    DEBUG_LOG("Starting brain log");

    std::stringstream writtenData;
    std::string fileName = "logs/Agent_Brain_Log_" + std::to_string(env_identifier) + ".txt";
    std::ifstream CurrentLog(fileName);
    if (CurrentLog)
    {
        writtenData << CurrentLog.rdbuf();
    }
    std::string data = writtenData.str() + "\n";
    DEBUG_LOG("Read log file " + std::to_string(env_identifier));
    for (InputNode *iN : inputNodes)
    {
        DEBUG_LOG("Starting input node log");
        data += outputBrain(iN, 0);
    }

    CurrentLog.close();

    std::ofstream updatedLog(fileName);
    DEBUG_LOG("Writing data to file");
    if (data == "")
    {
        data = "No brain found!";
    }
    updatedLog << data;

    updatedLog.close();

    DEBUG_LOG("Finished simulation cycle logging");
}

std::string Brain::outputBrain(InputNode *node, int depth)
{
    DEBUG_LOG("Outputting brain node at depth "
              << depth
              << " pointer "
              << node);

    if (!node)
    {
        DEBUG_LOG("Node is null");

        return "";
    }

    std::string data = "";

    std::string indent(
        depth * 2,
        ' ');

    data += indent;

    unsigned int key = node->getKey();

    DEBUG_LOG("Node key: "
              << key);

    if (key != 255)
    {
        DEBUG_LOG("Valid sense key");

        data +=
            std::to_string(key) + "|" + getSenses().at(key) + "|" + std::to_string(node->getWeight());
    }
    else
    {
        DEBUG_LOG("Invalid sense key");

        data +=
            std::to_string(key) + "|INVALID_SENSE_KEY";
    }

    if (node->getOutputNode())
    {
        DEBUG_LOG("Node has OutputNode");

        unsigned int outputKey =
            node->getOutputNode()->getKey();

        DEBUG_LOG("Output key: "
                  << outputKey);

        if (outputKey < getActions().size())
        {
            data +=
                "---" + std::to_string(outputKey) + "|" + getActions().at(outputKey) + "|" + std::to_string(node->getOutputNode()->getWeight());
        }
        else
        {
            DEBUG_LOG("Invalid output key");

            data +=
                "---" + std::to_string(outputKey) + "|INVALID_ACTION_KEY";
        }
    }

    data += "\n";

    DEBUG_LOG("Processing "
              << node->getInputNodes().size()
              << " child nodes");

    for (InputNode *cn :
         node->getInputNodes())
    {
        data += outputBrain(
            cn,
            depth + 1);
    }

    DEBUG_LOG("Finished outputting node at depth "
              << depth);

    return data;
}
