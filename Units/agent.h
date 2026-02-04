#pragma once
#include "brain.h"
#include <random>

class Agent{
public:





float health;
float bite; 
float energy;
//color
float speed; 
float sight; 
std::string facing;
Brain* brain;

Agent(){
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


void updateHealth(){
    // health is exponentially related to energy, if energy drops past a certain point, start losing health, otherwise gain health slowly
    // dropping point is directly related to max health value

    health += (exp(0.5*energy)-exp(0.1*health))/(exp(energy)-1);
    energy*=0.5;

    if (health < 0){
        health = 0;
    }
    
}





};