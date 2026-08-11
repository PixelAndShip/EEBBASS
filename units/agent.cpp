#include "agent.h"

Agent::Agent()
{

    processed = false;

    health = 200.0;

    energy = 200.0;

    plantDiet = 0.5;

    agentColor = {100, 100, 1, 255};

    x = 1000;
    y = 1000;

    speed = 1.0;
}

Agent::Agent(std::string fileName, bool iP, int iEID, float iH, float iE, float iPD, UnitColor iC, int ix, int iy, float iS) : brain(fileName, iEID)
{
    processed = iP;
    env_identifier = iEID;
    health = iH;
    energy = iE;
    plantDiet = iPD;
    agentColor = iC;
    x = ix;
    y = iy;
    speed = iS;
};

Agent::Agent(
    int identifier,
    float eRadiation,
    std::mt19937 &gen,
    std::uniform_int_distribution<> &dist,
    int childNodeCount,
    int brainDepth)
    : brain(identifier, eRadiation, gen, dist, childNodeCount, brainDepth)
{
    processed = false;

    env_identifier = identifier;

    health = 100.0;

    energy = 100.0;

    plantDiet = 0.5;

    agentColor = {100, 100, 1, 255};

    x = 1000;
    y = 1000;

    speed = 1.0;

    generateStart(gen);

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
    std::mt19937 &gen,
    int childNodeCount,
    int brainDepth)
    : brain(identifier, eRadiation, gen, iBrain, childNodeCount, brainDepth)
{

    env_identifier = identifier;

    health = iHealth;

    energy = iEnergy;

    plantDiet = iPlantDiet;

    speed = iSpeed;

    agentColor = {100, 100, 1, 255};

    x = 25;
    y = 25;

    updateColor();
}

void Agent::generateStart(std::mt19937 &gen)
{
}

void Agent::logAgent() const
{
    std::stringstream writtenData;
    std::string fileName = "logs/Environment_Log_" + std::to_string(env_identifier) + ".txt";
    std::ifstream CurrentLog(fileName);
    if (CurrentLog)
    {
        writtenData << CurrentLog.rdbuf();
    }
    std::string data = writtenData.str() + "\n";

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
    updatedLog << data;

    updatedLog.close();
}

std::string Agent::outputAgentStats() const
{
    std::string agentData;
    agentData += "A\n";
    agentData += 'P' + processed;
    agentData += "|I" + env_identifier;
    agentData += "|H" + std::to_string(health);
    agentData += "|E" + std::to_string(energy);
    agentData += "|D" + std::to_string(plantDiet);
    agentData += "|C{" + std::to_string(agentColor.red);
    agentData += ',' + std::to_string(agentColor.green);
    agentData += ',' + std::to_string(agentColor.blue);
    agentData += ',' + std::to_string(agentColor.transparency);
    agentData += "}|X" + std::to_string(x);
    agentData += "|Y" + std::to_string(y);
    agentData += "|S" + std::to_string(speed);
    return agentData;
}