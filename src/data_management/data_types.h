#pragma once
#include <unordered_map>
#include <string>
#include <algorithm>
#include "debug/debug.h"
#include <random>
struct UnitColor
{
    unsigned int red, green, blue, transparency;
};
inline unsigned int agentSize = 4;
inline unsigned int plantSize = 3;
inline unsigned int borderWidth = 100;
inline unsigned int borderHeight = 100;

inline std::random_device rd;
inline std::mt19937 gen(rd());

inline std::uniform_int_distribution<> dist(0, 100);
inline std::uniform_int_distribution<> rootNodeDist(0, 7);
inline std::uniform_int_distribution<> insideX(0, 100);
inline std::uniform_int_distribution<> insideY(0, 100);

inline std::vector<int> getDCoords() { return {-(int)agentSize * 2, -(int)agentSize, 0, (int)agentSize, (int)agentSize * 2}; }

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
