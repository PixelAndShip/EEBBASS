#pragma once
#include "units/brain_nodes/node_functions.h"
#include <queue>
#include <algorithm>

class Spider
{

public:
    std::unordered_map<std::string, Agent *> Agents;
    std::unordered_map<std::string, Plant *> Plants;
    // std::vector<std::string> processedAgents;
    std::vector<std::pair<std::string, OutputNode *>> actionQueue;
    std::vector<std::pair<std::string, Agent *>> pendingBirths;

    // std::unordered_map<std::string, bool> proximateCoords;

    float radiation;
    int maxRootNodes;
    int maxBrainLevel;
    int maxBrainChildNodes;
    unsigned int uniqueAgentID = 0;

    ~Spider();
    Spider(float rad = 0.5, int maxRN = 3, int maxBL = 3, int maxBCN = 4);

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