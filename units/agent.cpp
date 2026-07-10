#include "agent.h"

Agent::Agent()
{
    DEBUG_LOG("Starting default Agent constructor");

    health = 100.0;
    DEBUG_LOG("Set health: " << health);

    energy = 100.0;
    DEBUG_LOG("Set energy: " << energy);

    age = 1;
    DEBUG_LOG("Set age: " << age);

    agentColor = {100, 100, 1, 255};
    DEBUG_LOG("Set Agent color: "
              << agentColor.red << ", "
              << agentColor.green << ", "
              << agentColor.blue << ", "
              << agentColor.transparency);

    x = 25;
    y = 25;
    DEBUG_LOG("Set position: (" << x << ", " << y << ")");

    speed = 1.0;
    DEBUG_LOG("Set speed: " << speed);

    DEBUG_LOG("Finished default Agent constructor");
}

Agent::Agent(
    float eRadiation,
    std::mt19937 &gen,
    std::uniform_int_distribution<> &dist,
    int childNodeCount,
    int brainDepth)
    : brain(eRadiation, gen, dist, childNodeCount, brainDepth)
{
    DEBUG_LOG("Starting generated Agent constructor");

    health = 100.0;
    DEBUG_LOG("Set health: " << health);

    energy = 100.0;
    DEBUG_LOG("Set energy: " << energy);

    age = 1;
    DEBUG_LOG("Set age: " << age);

    agentColor = {100, 100, 1, 255};
    DEBUG_LOG("Set Agent color: "
              << agentColor.red << ", "
              << agentColor.green << ", "
              << agentColor.blue << ", "
              << agentColor.transparency);

    x = 25;
    y = 25;
    DEBUG_LOG("Set position: (" << x << ", " << y << ")");

    speed = 1.0;
    DEBUG_LOG("Set speed: " << speed);

    DEBUG_LOG("Generating Agent starting attributes");
    generateStart(gen);

    DEBUG_LOG("Finished generated Agent constructor");
}

Agent::Agent(
    float iHealth,
    float iEnergy,
    float iSpeed,
    const Brain &iBrain,
    float eRadiation,
    std::mt19937 &gen,
    int childNodeCount,
    int brainDepth)
    : brain(eRadiation, gen, iBrain, childNodeCount, brainDepth)
{
    DEBUG_LOG("Starting split spawn Agent constructor");

    health = iHealth / 2.0;
    DEBUG_LOG("Split health from " << iHealth << " to " << health);

    energy = iEnergy / 2.0;
    DEBUG_LOG("Split energy from " << iEnergy << " to " << energy);

    speed = iSpeed / 2.0;
    DEBUG_LOG("Split speed from " << iSpeed << " to " << speed);

    age = 1;
    DEBUG_LOG("Set age: " << age);

    agentColor = {100, 100, 1, 255};
    DEBUG_LOG("Set initial Agent color: "
              << agentColor.red << ", "
              << agentColor.green << ", "
              << agentColor.blue << ", "
              << agentColor.transparency);

    x = 25;
    y = 25;
    DEBUG_LOG("Set position: (" << x << ", " << y << ")");

    DEBUG_LOG("Finished split spawn Agent constructor");
}

void Agent::generateStart(std::mt19937 &gen)
{
    DEBUG_LOG("generateStart() called");

    // Currently no generation logic exists.
    // Add random starting attributes here when needed.

    DEBUG_LOG("Finished generateStart()");
}