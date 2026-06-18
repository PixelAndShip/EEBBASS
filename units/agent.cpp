#include "agent.h"

Agent::Agent()
{
    health = 1.0;
    energy = 1.0;
    age = 1;
    agentColor = {200, 200, 200, 255};
    x = 25;
    y = 25;
    speed = 1.0;
}

Agent::Agent(float eRadiation, std::mt19937 &gen, std::uniform_int_distribution<> &dist, int childNodeCount, int brainDepth) : brain(eRadiation, gen, dist, childNodeCount, brainDepth)
{
    health = 1.0;
    energy = 1.0;
    age = 1;
    agentColor = {200, 200, 200, 255};
    x = 25;
    y = 25;
    speed = 1.0;
    generateStart(gen);
    // calculate color
}

Agent::Agent(float iHealth, float iEnergy, float iSpeed, float iSight, Brain &iBrain, float eRadiation, std::mt19937 &gen, int childNodeCount, int brainDepth) : brain(eRadiation, gen, iBrain, childNodeCount, brainDepth)
{ // split spawn
    health = iHealth / 2.0;
    energy = iEnergy / 2.0;
    speed = iSpeed / 2.0;
}

void Agent::generateStart(std::mt19937 &gen)
{
    // https://www.geeksforgeeks.org/cpp/how-to-generate-random-number-in-range-in-cpp/
}
