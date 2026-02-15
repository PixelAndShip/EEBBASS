#pragma once
#include "brain.h"
#include <random>

class Agent{
public:


// dynamic coefficients:
float cHealth;
float cSpeed;

// dynamic values:
float health;
float bite; 
float energy;
//color
float speed; // adrenalin
int sight;



Brain* brain;

Agent();

Agent(float eRadiation, std::mt19937 gen, std::uniform_int_distribution<>& dist);// default spawn

Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain iBrain, float eRadiation);// split spawn

void generateStart(std::mt19937 gen);

void updateSpeed(float deltaEnergy);

void updateHealth(float deltaEnergy);



};