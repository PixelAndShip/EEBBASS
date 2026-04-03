#pragma once
#include "brain.h"
#include <random>



class Agent{
public:



// dynamic values:
float health;
float bite; 
float energy;

int red,green,blue,transparency;
int x,y;

float speed; // adrenalin
int sight;



Brain brain;

Agent();

Agent(float eRadiation, std::mt19937& gen, std::uniform_int_distribution<>& dist);// default spawn

Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain& iBrain, float eRadiation);// split spawn

void generateStart(std::mt19937& gen);





};