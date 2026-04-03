#pragma once
#include "Units/agent.h"

class Spider{
public:

/*

{0,"SeeSomething"},
{1,"SeeColor"},
{2,"SeeInRangeSomething"},
{3,"SeeInRangeColor"},
{4,"EnergyCountAboveSet"},
{5,"EnergyCountBelowSet"},
{6,"HealthCountAboveSet"},
{7,"HealthCountBelowSet"}

*/

static void seeSomething();

static void seeColor();

static void seeInRangeSomething();

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