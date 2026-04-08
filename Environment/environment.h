#pragma once
#include <vector>
#include <iostream>
#include <random>
#include "raylib.h"
#include "spider.h"

class Environment{
public:
float radiation;
int carbon_count;

std::mt19937 gen;
std::uniform_int_distribution<> dist;
std::unordered_map<std::string,Agent*> agents = {};
std::unordered_map<std::string,Plant*> plants = {};
// plant count
// Area

Spider spider = Spider();

void managePlantCount(); // makes sure sim does not crash

void manageAgentCount(); // makes sure sim does not crash

void manageMoment(); /*
loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
this means each moment needs to be subdivided into submoments:
determening which agent gets to perform their external action first is determined by which agent has the higher speed stat

*/
void manageSubMoment(std::string coords, std::unordered_map<std::string,bool>* managedAgentCoordinates);

std::vector<std::string> agentProximityCheck(std::string coords);

OutputNode getAction(InputNode* parentNode);

void makeWindow();
};