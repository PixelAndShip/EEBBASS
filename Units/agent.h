#pragma once
#include "brain.h"
#include <random>

class Agent{
public:

float healthCoef;
float energyCoef;
float speedCoef;
float sightCoef;


float health;
float energy;
//color
float speed;
float sight;
Brain* brain;

Agent(){}

Agent(float eRadiation)// default spawn
{
    generateCoefs();

    
    brain = new Brain(eRadiation);
    // calculate color
}
Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain iBrain, float eRadiation){ // split spawn

    float strength = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    


    brain = new Brain(eRadiation,iBrain);
}

void generateCoefs(){
    float total_coef = 10.0;
    health = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*10;
    // rest is 10 - each calculated attribute value
    
}


void updateHealth(){

    health += energy*energy - 2; // health updated on negative parabola dependency of energy
    if (health < 0){
        health = 0;
    }
    
}
};