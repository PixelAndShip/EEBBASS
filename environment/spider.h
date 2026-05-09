#pragma once
#include "units/agent.h"
#include "units/plant.h"
#include <queue>
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

    */

    bool seeSomething();

    bool seeColor(int red, int green, int blue, int transparency, int setRed, int setGreen, int setBlue, int setTransparency);

    bool energyCountAboveSet(float energy, float setAmount);

    bool energyCountBelowSet(float energy, float setAmount);

    bool healthCountAboveSet(float health, float setAmount);

    bool healthCountBelowSet(float health, float setAmount);

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

    void bite(auto *Opponent, float energyCost);

    void move(Agent *Self, char Direction);

    void setProximities(std::string coords);
    std::vector<std::string> getProximateAgents(std::string coords);
    std::vector<std::string> getProximatePlants(std::string coords);

    OutputNode *getAction(InputNode *parentNode);
    void manageSubMoment();
};