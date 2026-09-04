#pragma once
#include <unordered_map>
#include <string>
#include <algorithm>
#include "debug/debug.h"
#include <random>
#include <filesystem>
struct UnitColor
{
    unsigned int red, green, blue, transparency;
};

enum class UserInputCase
{
    Default,
    Visual,
    Multithread,
    Custom
};

enum FloatInput
{
    eRad,
    cull,
    floatInputCount
};

enum IntInput
{
    maxA,
    maxP,
    iT,
    maxCultivate,
    maxCycle,
    cb,
    maxBL,
    maxBCN,
    rootNodesCount,
    fps,
    intInputCount
};

inline unsigned int agentSize = 4;
inline unsigned int plantSize = 3;
inline unsigned int borderWidth = 700;
inline unsigned int borderHeight = 700;

inline std::random_device rd;
inline std::mt19937 gen(rd());

inline std::uniform_int_distribution<> dist(0, 100);
inline std::uniform_int_distribution<> rootNodeDist(0, 7);
inline std::uniform_int_distribution<> insideX(0, 100);
inline std::uniform_int_distribution<> insideY(0, 100);

inline std::vector<int> getDCoords() { return {-2, -1, 0, 1, 2}; }

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
        {6, "ExpendEnergyOnHealth"},
        {7, "ExpendEnergyOnSpeed"},
        {8, "BiteColor"}};
    return Actions;
}
