#pragma once
#include "brain.h"
#include <random>

class Agent{
public:
float health;
float energy;
//color
float speed;
float sight;
Brain* brain;

Agent(){}

Agent(float eRadiation)// default spawn
{
    float total_coef = 10.0;

    health = 1;
    energy = 1;
    speed = 1;
    brain = new Brain(eRadiation);
    // calculate color
}
Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain iBrain, float eRadiation){ // split spawn

    float strength = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    


    brain = new Brain(eRadiation,iBrain);
}
};