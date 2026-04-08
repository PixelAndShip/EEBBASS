#pragma once
#include "Units/agent.h"
#include "Units/plant.h"
class Spider{
public:

/*

{0,"SeeSomething"},
{1,"SeeColor"},
{2,"EnergyCountAboveSet"},
{3,"EnergyCountBelowSet"},
{4,"HealthCountAboveSet"},
{5,"HealthCountBelowSet"}

*/

bool seeSomething(std::vector<std::string>& proximateCoords,std::unordered_map<std::string,Agent*>& Agents, char& unit);

bool seeColor();





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

void updateSpeed(float deltaEnergy, Agent* Self);

void updateHealth(float deltaEnergy, Agent* Self);

void bite(Agent* Opponent, float energyCost);

void move(Agent* Self, char Direction);

void updateCHealth(Agent* Self, float Amount);

};