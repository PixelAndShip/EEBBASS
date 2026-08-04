#pragma once
#include "units/brain_nodes/node_functions.h"
#include <queue>
#include <algorithm>
struct PendingAction
{
    std::string coords;
    OutputNode *action;
};

class Spider
{

public:
    std::unordered_map<std::string, Agent *> Agents;
    std::unordered_map<std::string, Plant *> Plants;
    // std::vector<std::string> processedAgents;
    std::vector<PendingAction> actionQueue;
    std::vector<std::pair<std::string, Agent *>> pendingBirths;

    // std::unordered_map<std::string, bool> proximateCoords;

    float radiation;
    int maxBrainLevel;
    int maxBrainChildNodes;

    int terrariumWidth;
    int terrariumHeight;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;

    Spider();
    ~Spider();
    Spider(float rad, int maxBL = 3, int maxBCN = 4, int terW = 800, int terH = 800);

    // void setProximities(std::string coords);

    void setNextAgent(std::string oldCoords, std::string newCoords, Agent *Self);

    bool manageSense(std::string AgentCoordinates, InputNode *Sense);
    OutputNode *getAction(std::string AgentCoordinates, InputNode *parentNode);

    std::vector<std::string> sortAgentsBySpeed(std::vector<std::string> agents);

    void splitNewAgent(std::string ParentCoords);

    void manageAction(std::string AgentCoordinates, OutputNode *ActionNode);
    void manageSubMoment();

    bool borderCheck(std::string coords);
};