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
float speed; 
float sight;

// dynamic states:
std::string facing;

Brain* brain;

Agent(){
    cHealth = 0.5;
    cSpeed = 1/(1-cHealth);
    health = 1.0;
    bite = 1.0;
    energy = 1.0;
    speed = 1.0;
    sight = 1.0;
    
}

Agent(float eRadiation, std::mt19937 gen)// default spawn
{
    generateStart(gen);
    
    brain = new Brain(eRadiation);
    // calculate color
}
Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain iBrain, float eRadiation){ // split spawn

   
    


    brain = new Brain(eRadiation,iBrain);
}

void generateStart(std::mt19937 gen){
    // https://www.geeksforgeeks.org/cpp/how-to-generate-random-number-in-range-in-cpp/
    
   
   
    
}

void updateSpeed(float deltaEnergy){
    speed = 1/(3*cSpeed)*deltaEnergy-1;
    if(speed <0){
        speed = 0;
    }
}


void updateHealth(float deltaEnergy){
    if (cHealth <0.2){
        cHealth = 0.2;
    }
    else if(cHealth >1){
        cHealth = 1;
    }

    float deltaHealth = (1/10*cHealth)*health*deltaEnergy-health;
    if(deltaHealth < -2){
        deltaHealth = -2;
    }
    energy-=deltaEnergy;
    health+=deltaHealth;
    if (health < 0){
        health = 0;
    }
    
    
}





};