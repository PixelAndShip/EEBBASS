#pragma once
#include "data_management/data_types.h"
#include <random>
class OutputNode
{
private:
    float weight = 1;
    float energyCost = 1;
    unsigned int key = 255;

public:
    OutputNode() {};
    OutputNode(std::mt19937 &gen)
    {
        std::uniform_int_distribution<> weightDist(0, 99);
        std::uniform_int_distribution<> energyCostDist(1, 10);
        std::uniform_int_distribution<> actionsDist(0, getActions().size() - 1);

        weight = weightDist(gen) / 100.0;
        energyCost = energyCostDist(gen);
        key = actionsDist(gen);
    }

    void setWeight(float iW)
    {
        weight = iW;
    }
    void setEnergyCost(float iEC)
    {
        energyCost = iEC;
    }
    void setKey(unsigned int iK)
    {
        key = iK;
    }
    float getWeight()
    {
        return weight;
    }
    float getEnergyCost()
    {
        return energyCost;
    }
    float getKey()
    {
        return key;
    }
};