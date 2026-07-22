#include "agent.h"

Agent::Agent()
{

    health = 100.0;

    energy = 100.0;

    plantDiet = 0.5;

    agentColor = {100, 100, 1, 255};

    x = 25;
    y = 25;

    speed = 1.0;
}

Agent::Agent(
    int identifier,
    float eRadiation,
    std::mt19937 &gen,
    std::uniform_int_distribution<> &dist,
    int childNodeCount,
    int brainDepth)
    : brain(identifier, eRadiation, gen, dist, childNodeCount, brainDepth)
{

    env_identifier = identifier;

    health = 200.0;

    energy = 200.0;

    plantDiet = 0.5;

    agentColor = {100, 100, 1, 255};

    x = 25;
    y = 25;

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
    DEBUG_LOG("generateStart() called");

    // Currently no generation logic exists.
    // Add random starting attributes here when needed.

    DEBUG_LOG("Finished generateStart()");
}