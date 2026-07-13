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
    OutputNode()
    {
        DEBUG_LOG("Default OutputNode constructor called");
    };

    OutputNode(std::mt19937 &gen, OutputNode *copyNode)
    {
        DEBUG_LOG("Starting OutputNode copy constructor");

        // Initialize mutation
        std::uniform_int_distribution<> mutationDist(-2, 2);
        std::uniform_int_distribution<> keyMutationChance(0, 100);

        weight = std::clamp(copyNode->getWeight() + mutationDist(gen) / 100.0f, 0.01f, 0.99f);
        DEBUG_LOG("Copied and mutated weight to: " << weight);

        energyCost = std::clamp(copyNode->getEnergyCost() + mutationDist(gen), 1.0f, 255.0f);
        DEBUG_LOG("Copied and mutated energyCost to: " << energyCost);

        unitColor = copyNode->getUnitColor();
        DEBUG_LOG("Copied color: "
                  << unitColor.red << ", "
                  << unitColor.green << ", "
                  << unitColor.blue << ", "
                  << unitColor.transparency);

        key = copyNode->getKey();
        DEBUG_LOG("Copied key: " << key);

        int mutatedKeyChance = keyMutationChance(gen);
        DEBUG_LOG("Key mutation roll: " << mutatedKeyChance);

        if (mutatedKeyChance <= 5)
        {
            int newKey = static_cast<int>(key) + mutationDist(gen);

            key = static_cast<unsigned int>(
                std::clamp(
                    newKey,
                    0,
                    static_cast<int>(getActions().size()) - 1));

            DEBUG_LOG("Key mutated to: " << key);
        }

        DEBUG_LOG("Finished OutputNode copy constructor");
    }

    OutputNode(std::mt19937 &gen)
    {
        DEBUG_LOG("Starting random OutputNode constructor");

        std::uniform_int_distribution<> weightDist(0, 99);
        std::uniform_int_distribution<> colorDist(0, 255);
        std::uniform_int_distribution<> energyCostDist(1, 10);
        std::uniform_int_distribution<> actionsDist(0, getActions().size() - 1);

        unitColor = {
            (unsigned int)colorDist(gen),
            (unsigned int)colorDist(gen),
            (unsigned int)colorDist(gen),
            (unsigned int)colorDist(gen)};

        DEBUG_LOG("Generated color: "
                  << unitColor.red << ", "
                  << unitColor.green << ", "
                  << unitColor.blue << ", "
                  << unitColor.transparency);

        weight = weightDist(gen) / 100.0;
        DEBUG_LOG("Generated weight: " << weight);

        energyCost = energyCostDist(gen);
        DEBUG_LOG("Generated energyCost: " << energyCost);

        key = actionsDist(gen);
        DEBUG_LOG("Generated key: " << key);

        DEBUG_LOG("Finished random OutputNode constructor");
    }

    void setWeight(float iW)
    {
        DEBUG_LOG("Setting weight from " << weight << " to " << iW);
        weight = iW;
    }

    void setEnergyCost(float iEC)
    {
        DEBUG_LOG("Setting energyCost from " << energyCost << " to " << iEC);
        energyCost = iEC;
    }

    void setKey(unsigned int iK)
    {
        DEBUG_LOG("Setting key from " << key << " to " << iK);
        key = iK;
    }

    void setUnitColor(UnitColor iC)
    {
        DEBUG_LOG("Setting color from ("
                  << unitColor.red << ", "
                  << unitColor.green << ", "
                  << unitColor.blue << ", "
                  << unitColor.transparency << ") to ("
                  << iC.red << ", "
                  << iC.green << ", "
                  << iC.blue << ", "
                  << iC.transparency << ")");

        unitColor = iC;
    }

    float getWeight()
    {
        DEBUG_LOG("Getting OutputNode weight: " << weight);
        return weight;
    }

    float getEnergyCost()
    {
        DEBUG_LOG("Getting OutputNode energyCost: " << energyCost);
        return energyCost;
    }

    unsigned int getKey()
    {
        DEBUG_LOG("Getting OutputNode key: " << key);
        return key;
    }

    UnitColor getUnitColor()
    {
        DEBUG_LOG("Getting OutputNode color: "
                  << unitColor.red << ", "
                  << unitColor.green << ", "
                  << unitColor.blue << ", "
                  << unitColor.transparency);

        return unitColor;
    }
};