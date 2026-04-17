#pragma once
#include "Units/agent.h"
#include "Units/plant.h"

class Spider
{

public:
    std::vector<std::string> proximateAgents;
    std::unordered_map<std::string, Agent *> Agents;

    std::vector<std::string> proximatePlants;
    std::unordered_map<std::string, Plant *> Plants;
    /*

    {0,"SeeSomething"},
    {1,"SeeColor"},
    {2,"EnergyCountAboveSet"},
    {3,"EnergyCountBelowSet"},
    {4,"HealthCountAboveSet"},
    {5,"HealthCountBelowSet"}

    */

    bool seeSomething(char &unit);

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

    void bite(Agent *Opponent, float energyCost);

    void move(Agent *Self, char Direction);

    void updateCHealth(Agent *Self, float Amount);
};