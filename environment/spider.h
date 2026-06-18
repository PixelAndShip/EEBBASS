#pragma once
#include "units/agent.h"
#include "units/plant.h"
#include <queue>
#include <algorithm>
class Spider
{

public:
    std::unordered_map<std::string, Agent *> NextAgents;
    std::unordered_map<std::string, Agent *> NextPlants;
    std::unordered_map<std::string, Agent *> PastAgents;
    std::unordered_map<std::string, Plant *> PastPlants;

    std::unordered_map<std::string, bool> proximateCoords;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;

    Spider();
    /*

    {0,"SeeSomething"},
    {1,"SeeColor"},
    {2,"EnergyCountAboveSet"},
    {3,"EnergyCountBelowSet"},
    {4,"HealthCountAboveSet"},
    {5,"HealthCountBelowSet"}
    {6, "AgeCountAboveSet"},
    {7, "AgeCountBelowSet"}};
    */

    bool seeSomething(std::string AgentCoordinates);

    bool seeColor(std::string AgentCoordinates, UnitColor setC);

    bool energyCountAboveSet(float energy, float setAmount);

    bool energyCountBelowSet(float energy, float setAmount);

    bool healthCountAboveSet(float health, float setAmount);

    bool healthCountBelowSet(float health, float setAmount);

    bool ageCountAboveSet(float age, float setAmount);

    bool ageCountBelowSet(float age, float setAmount);

    /*

{0,"MoveLeft"},
{1,"MoveRight"},
{2,"MoveUp"},
{3,"MoveDown"},
{4,"Bite"},
{5,"Split"},
{6,"ExpendEnergyOnHealth"}, // passive action
{7,"ExpendEnergyOnSpeed"} // passive action

*/

    void updateSpeed(float deltaEnergy, Agent *Self);

    void updateHealth(float deltaEnergy, Agent *Self);

    void bite(std::string AgentCoordinates, float energyCost);

    void move(std::string AgentCoordinates, char Direction);

    void biteColor(std::string AgentCoordinates, UnitColor Target, float energyCost);

    void setProximities(std::string coords);
    std::vector<std::string> getProximateAgents(std::string coords);
    std::string getSplitCoords(std::string ParentCoords);
    std::vector<std::string> getProximatePlants(std::string coords);

    void setNextAgent(std::string coords, Agent *Self);

    bool manageSense(std::string AgentCoordinates, InputNode *Sense);
    OutputNode *getAction(std::string AgentCoordinates, InputNode *parentNode);

    std::vector<std::string> sortAgentsBySpeed(std::vector<std::string> agents);

    void splitNewAgent(std::string ParentCoords);

    void manageAction(std::string AgentCoordinates, OutputNode *ActionNode);
    void manageSubMoment();
};