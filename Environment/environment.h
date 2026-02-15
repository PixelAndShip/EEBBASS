#pragma once
#include <vector>
#include "Units/agent.h"

class Environment{
public:
float radiation;
int carbon_count;
std::vector<Agent> agentCount;
// plant count
// Area

void ManagePlantCount(); // makes sure sim does not crash

void ManageAgentCount(); // makes sure sim does not crash

void manageMoment(); /*
loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
this means each moment needs to be subdivided into submoments:
determening which agent gets to perform their external action first is determined by which agent has the higher speed stat

*/
};