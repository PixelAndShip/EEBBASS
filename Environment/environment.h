#pragma once
#include <vector>
#include "Units/agent.h"
#include "raylib.h"
#include <iostream>
class Environment{
public:
float radiation;
int carbon_count;
std::unordered_map<std::string,Agent*> Agents = {};
// plant count
// Area

void ManagePlantCount(); // makes sure sim does not crash

void ManageAgentCount(); // makes sure sim does not crash

void manageMoment(); /*
loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
this means each moment needs to be subdivided into submoments:
determening which agent gets to perform their external action first is determined by which agent has the higher speed stat

*/
void manageSubMoment(std::string coords, std::unordered_map<std::string,bool>* managedAgentCoordinates);

std::vector<std::string> agentProximityCheck(std::string coords);

void makeWindow();
};