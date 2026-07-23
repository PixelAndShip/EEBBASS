#pragma once
#include <unordered_map>
#include <string>
#include <algorithm>
#include "data_management/debug.h"
struct UnitColor
{
    unsigned int red, green, blue, transparency;
};

inline const std::unordered_map<int, std::string> &getSenses()
{

    static const std::unordered_map<int, std::string> Senses = {
        {0, "SeeSomething"},
        {1, "SeeColor"},
        {2, "EnergyCountAboveSet"},
        {3, "EnergyCountBelowSet"},
        {4, "HealthCountAboveSet"},
        {5, "HealthCountBelowSet"}};
    return Senses;
}

inline const std::unordered_map<int, std::string> &getActions()
{

    static const std::unordered_map<int, std::string> Actions = {
        {0, "MoveLeft"},
        {1, "MoveRight"},
        {2, "MoveUp"},
        {3, "MoveDown"},
        {4, "Bite"},
        {5, "Split"},
        {6, "ExpendEnergyOnHealth"}, // passive action
        {7, "ExpendEnergyOnSpeed"},  // passive action
        {8, "BiteColor"}};
    return Actions;
}
