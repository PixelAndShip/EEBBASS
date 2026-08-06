#include "brain.h"

Brain::Brain()
{
}

Brain::Brain(std::string fileName, int iEID)
{
    env_identifier = iEID;
    constructCustomBrain(fileName);
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

void Brain::logBrain() const
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

std::string Brain::outputBrain(InputNode *node, int depth) const
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

    // float weight = node->getWeight();
    // float setAmount = node->getSetAmount();
    // UnitColor unitColor = node->getUnitColor();
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

void Brain::constructCustomBrain(std::string fileName)
{
    std::string line, allData = "";
    std::ifstream customBrainFile(fileName);

    if (!customBrainFile.is_open())
    {
        return;
    }

    while (getline(customBrainFile, line))
    {
        constructCustomNode(line);
    }

    customBrainFile.close();
}

// [1]+weight/setAmount/key/{unitColor}
// [1.1]...-weight/energyCost/key/{unitColor}

void Brain::constructCustomNode(std::string line)
{
    auto idS = line.find("[");
    auto idE = line.find("]");

    if (idS == std::string::npos or idE == std::string::npos)
    {
        return;
    }

    std::string id = line.substr(idS + 1, idE - idS - 1);

    if (idE + 1 >= line.size())
    {
        return;
    }
    char nodeType = line[idE + 1];

    bool isInput = nodeType == '+';
    bool isOutput = nodeType == '-';

    if (!isInput and !isOutput)
    {
        return;
    }

    float weight = 0.0;
    float energyCost = 0.0;
    float setAmount = 0.0;
    unsigned int key = 255;

    UnitColor unitColor = {0, 0, 0, 255};

    size_t pos = idE + 2;

    while (pos < line.size())
    {
        char indicator = line[pos];

        if (indicator == 'K')
        {

            size_t start = pos + 1;
            size_t end = line.find('/', start);

            if (end == std::string::npos)
            {
                end = line.size();
            }
            key = std::stoi(line.substr(start, end - start));

            pos = end;
        }
        else if (indicator == 'W')
        {

            size_t start = pos + 1;
            size_t end = line.find('/', start);

            std::string value = line.substr(start, end - start);

            weight = std::stof(value);

            pos = end;
        }
        else if (indicator == 'E')
        {

            size_t start = pos + 1;
            size_t end = line.find('/', start);

            std::string value = line.substr(start, end - start);

            if (isInput)
            {
                setAmount = std::stof(value);
            }
            else
            {
                energyCost = std::stof(value);
            }
            pos = end;
        }
        else if (indicator == 'C')
        {

            size_t start = line.find('{', pos);

            if (start == std::string::npos)
            {
                break;
            }
            size_t end = line.find('}', start);

            if (end == std::string::npos)
            {
                break;
            }

            std::string colorString = line.substr(start + 1, end - start - 1);

            std::vector<int> values;
            std::string num;

            for (char c : colorString)
            {
                if (isdigit(c))
                {
                    num += c;
                }
                else if (!num.empty())
                {
                    values.push_back(std::stoi(num));
                    num = "";
                }
            }

            if (!num.empty())
                values.push_back(std::stoi(num));

            if (values.size() == 4)
            {
                unitColor = {
                    (unsigned char)values[0],
                    (unsigned char)values[1],
                    (unsigned char)values[2],
                    (unsigned char)values[3]};
            }

            pos = end + 1;
        }
        else
        {
            pos++;
        }
    }

    if (isInput)
    {
        InputNode *iN = new InputNode(weight, setAmount, key, unitColor);
        if (id.size() == 1)
        {
            inputNodes.push_back(iN);
        }
        else
        {
            std::string parentId = "";

            size_t pos = id.rfind('.');

            if (pos != std::string::npos)
            {
                parentId = id.substr(0, pos);
            }
            customNodes[parentId]->appendInputNode(iN);
        }
        customNodes[id] = iN;
    }
    else
    {
        OutputNode *oN = new OutputNode(weight, energyCost, key, unitColor);
        std::string parentId = "";

        size_t pos = id.rfind('.');

        if (pos != std::string::npos)
        {
            parentId = id.substr(0, pos);
        }
        customNodes[parentId]->setOutputNode(oN);
    }
}
