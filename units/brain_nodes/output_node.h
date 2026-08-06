#pragma once
#include "data_management/data_types.h"

#include <random>

class OutputNode
{
private:
    float weight = 1;
    float energyCost = 1;
    unsigned int key = 255;
    UnitColor unitColor = {255, 255, 255, 255};

public:
    OutputNode() {

    };

    OutputNode(float iW, float iE, unsigned int iK, UnitColor iU)
    {
        weight = iW;
        energyCost = iE;
        key = iK;
        unitColor = iU;
    }

    OutputNode(std::mt19937 &gen, OutputNode *copyNode)
    {

        // Initialize mutation
        std::uniform_int_distribution<> mutationDist(-2, 2);
        std::uniform_int_distribution<> keyMutationChance(0, 100);

        weight = std::clamp(copyNode->getWeight() + mutationDist(gen) / 100.0f, 0.01f, 0.99f);

        energyCost = std::clamp(copyNode->getEnergyCost() + mutationDist(gen), 1.0f, 255.0f);

        unitColor = copyNode->getUnitColor();

        key = copyNode->getKey();

        int mutatedKeyChance = keyMutationChance(gen);

        if (mutatedKeyChance <= 5)
        {
            int newKey = static_cast<int>(key) + mutationDist(gen);

            key = static_cast<unsigned int>(
                std::clamp(
                    newKey,
                    0,
                    static_cast<int>(getActions().size()) - 1));
        }
    }

    OutputNode(std::mt19937 &gen)
    {

        std::uniform_int_distribution<> weightDist(0, 99);
        std::uniform_int_distribution<> colorDist(0, 255);
        std::uniform_int_distribution<> energyCostDist(1, 50);
        std::uniform_int_distribution<> actionsDist(0, getActions().size() - 1);

        unitColor = {
            (unsigned int)colorDist(gen),
            (unsigned int)colorDist(gen),
            (unsigned int)colorDist(gen),
            (unsigned int)colorDist(gen)};

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

    void setUnitColor(UnitColor iC)
    {

        unitColor = iC;
    }

    float getWeight()
    {

        return weight;
    }

    float getEnergyCost()
    {

        return energyCost;
    }

    unsigned int getKey()
    {

        return key;
    }

    UnitColor getUnitColor()
    {

        return unitColor;
    }
};