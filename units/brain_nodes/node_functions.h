#pragma once
#include "agent.h"
#include "plant.h"

/*

{0,"SeeSomething"},
{1,"SeeColor"},
{2,"EnergyCountAboveSet"},
{3,"EnergyCountBelowSet"},
{4,"HealthCountAboveSet"},
{5,"HealthCountBelowSet"}

*/

extern bool seeSomething(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates);

extern bool seeColor(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, UnitColor setC);

extern bool energyCountAboveSet(float energy, float setAmount);

extern bool energyCountBelowSet(float energy, float setAmount);

extern bool healthCountAboveSet(float health, float setAmount);

extern bool healthCountBelowSet(float health, float setAmount);

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

void bite(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, float energyCost);

void move(const std::unordered_map<std::string, Agent *> *Agents, std::string AgentCoordinates, char Direction);

void biteColor(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string AgentCoordinates, UnitColor Target, float energyCost);

extern std::vector<std::string> getProximateAgents(const std::unordered_map<std::string, Agent *> *Agents, std::string coords);
extern std::string getSplitCoords(const std::unordered_map<std::string, Agent *> *Agents, const std::unordered_map<std::string, Plant *> *Plants, std::string ParentCoords);
extern std::vector<std::string> getProximatePlants(const std::unordered_map<std::string, Plant *> *Plants, std::string coords);