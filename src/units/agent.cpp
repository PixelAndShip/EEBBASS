#include "agent.h"

Agent::Agent(std::string selfData, std::string brainData, int iEID) : brain(brainData, iEID)
{
    env_identifier = iEID;
    constructCustomAgent(selfData);
}

Agent::Agent(
    int identifier,
    float eRadiation,
    int childNodeCount,
    int brainDepth)
    : brain(identifier, eRadiation, childNodeCount, brainDepth)
{
    processed = false;

    env_identifier = identifier;

    generateStart();

    updateColor();
}

Agent::Agent(
    int identifier,
    float iHealth,
    float iEnergy,
    float iPlantDiet,
    float iSpeed,
    const Brain &iBrain,
    float eRadiation,
    int maxRootNodesCount,
    int childNodeCount,
    int brainDepth)
    : brain(identifier, eRadiation, iBrain, maxRootNodesCount, childNodeCount, brainDepth)
{

    env_identifier = identifier;

    health = iHealth;

    energy = iEnergy;

    plantDiet = iPlantDiet;

    speed = iSpeed;

    agentColor = {100, 100, 1, 255};

    x = 25;
    y = 25;
    mutateStart();
    updateColor();
}

void Agent::generateStart()
{

    std::uniform_int_distribution<> startDist(0, 100);

    health = 100;
    energy = 100;
    plantDiet = startDist(gen) / 100.0;
    speed = startDist(gen);
}

void Agent::mutateStart()
{
    std::uniform_int_distribution<> mutateDist(-10, 10);

    health += mutateDist(gen);
    if (health < 0)
    {
        health = 0;
    }
    energy += mutateDist(gen);
    if (energy < 0)
    {
        energy = 0;
    }
    plantDiet += mutateDist(gen) / 100.0;
    if (plantDiet < 0)
    {
        plantDiet = 0;
    }
    else if (plantDiet > 1)
    {
        plantDiet = 1;
    }
    speed += mutateDist(gen);
    if (speed < 0)
    {
        speed = 1;
    }
}

void Agent::logAgent(int iIteration, std::string fileName) const
{
    std::stringstream writtenData;
    bool logFile = false;
    if (fileName == "")
    {
        logFile = true;
        fileName = "logs/Environment_Log_" + std::to_string(env_identifier) + ".txt";
    }
    std::ifstream CurrentLog(fileName);
    if (CurrentLog)
    {
        writtenData << CurrentLog.rdbuf();
    }
    std::string data = writtenData.str() + "\n";
    if (logFile)
    {
        data += std::to_string(iIteration) + "\n";
    }
    data += outputAgentStats();

    CurrentLog.close();

    std::ofstream updatedLog(fileName);

    if (data == "")
    {
        data = "No agent data found!";
    }
    data += "\n";
    data += "B\n";
    data += brain.logBrain();
    data += "=";
    updatedLog
        << data;

    updatedLog.close();
}

std::string Agent::outputAgentStats() const
{
    std::string agentData;
    agentData += "A\n";
    agentData += 'P' + std::to_string(processed);
    agentData += "/I" + std::to_string(env_identifier);
    agentData += "/H" + std::to_string(health);
    agentData += "/E" + std::to_string(energy);
    agentData += "/D" + std::to_string(plantDiet);
    agentData += "/C{" + std::to_string(agentColor.red);
    agentData += ',' + std::to_string(agentColor.green);
    agentData += ',' + std::to_string(agentColor.blue);
    agentData += ',' + std::to_string(agentColor.transparency);
    agentData += "}/X" + std::to_string(x);
    agentData += "/Y" + std::to_string(y);
    agentData += "/S" + std::to_string(speed);
    return agentData;
}

void Agent::constructCustomAgent(std::string data)
{
    size_t pos = 0;

    while (pos < data.size())
    {
        char indicator = data[pos];
        switch (indicator)
        {
        case 'P':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            processed = data.substr(start, end - start) == "1";
            pos = end;
            break;
        }
        case 'I':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            env_identifier = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'H':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            health = std::stof(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'E':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            energy = std::stof(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'D':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            plantDiet = std::stof(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'C':
        {
            size_t start = pos + 2;
            size_t end = data.find('}', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            std::string colorData = data.substr(start, end - start);
            std::string color = "";
            int colorId = 0;
            for (char unit : colorData)
            {
                if (unit == ',')
                {
                    switch (colorId)
                    {
                    case 0:
                        agentColor.red = std::stoi(color);
                        break;
                    case 1:
                        agentColor.green = std::stoi(color);
                        break;
                    case 2:
                        agentColor.blue = std::stoi(color);
                        break;
                    case 3:
                        agentColor.transparency = std::stoi(color);
                        break;
                    default:
                        break;
                    }
                    colorId++;
                    color = "";
                }
                else
                {
                    color += unit;
                }
            }
            if (!color.empty())
            {
                agentColor.transparency = std::stoi(color);
            }
            pos = end + 1;
            break;
        }
        case 'X':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            x = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'Y':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            y = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'S':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            speed = std::stof(data.substr(start, end - start));
            pos = end;
            break;
        }
        default:
            pos++;
            break;
        }
    }
}