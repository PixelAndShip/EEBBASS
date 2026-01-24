#pragma once
#include "brain.h"


class Agent{
public:
float health;
float energy;
//color
float speed;
Brain* brain;

Agent(){}

Agent(float eRadiation)// default spawn
{
    health = 1;
    energy = 1;
    speed = 1;
    brain = new Brain(eRadiation);
    // calculate color
}
Agent(float iHealth, float iEnergy,float iSpeed, Brain iBrain, float eRadiation): health(iHealth), energy(iEnergy), speed(iSpeed){ // split spawn
    brain = new Brain(eRadiation,iBrain);
}
};