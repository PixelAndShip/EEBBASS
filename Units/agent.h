#pragma once
#include "brain.h"
#include <random>

class Agent{
public:

float maxHealth;
float maxBite;
float maxSpeed;
float maxSight;



float health;
float energy;
//color
float speed;
float sight;
Brain* brain;

Agent(){}

Agent(float eRadiation)// default spawn
{
    generateMax();

    
    brain = new Brain(eRadiation);
    // calculate color
}
Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain iBrain, float eRadiation){ // split spawn

    float strength = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    


    brain = new Brain(eRadiation,iBrain);
}

void generateMax(){
    

    float total_max = 10.0;
    maxHealth, health = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*10;
    // rest is 10 - each calculated attribute value
    
}


void updateHealth(){


    if (health < 0){
        health = 0;
    }
    
}
};