#pragma once
#include "DataTypes/datatypes.h"

class OutputNode
{
private:
    float weight;
    float energyCost;
    unsigned int key;

public:
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