#pragma once
#include <unordered_map>
#include <string>
#include <algorithm>
#include "data_management/debug.h"
struct UnitColor
{
    unsigned int red, green, blue, transparency;
};
const unsigned int agentSize = 4;
const unsigned int plantSize = 3;

inline const std::unordered_map<int, std::string> &getSenses()
{

    static const std::unordered_map<int, std::string> Senses = {
        {0, "TouchSomething"},
        {1, "TouchColor"},
        {2, "EnergyCountAboveSet"},
        {3, "EnergyCountBelowSet"},
        {4, "HealthCountAboveSet"},
        {5, "HealthCountBelowSet"},
        {6, "SeeSomethingLeft"},
        {7, "SeeSomethingRight"},
        {8, "SeeSomethingUp"},
        {9, "SeeSomethingDown"},
        {10, "SeeColorLeft"},
        {11, "SeeColorRight"},
        {12, "SeeColorUp"},
        {13, "SeeColorDown"}};
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
