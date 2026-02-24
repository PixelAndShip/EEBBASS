#include "agent.h"


Agent::Agent(){
    cHealth = 0.5;
    cSpeed = 1/(cHealth);
    health = 1.0;
    bite = 1.0;
    energy = 1.0;
    speed = 1.0;
    sight = 1;
}

Agent::Agent(float eRadiation, std::mt19937 gen, std::uniform_int_distribution<>& dist){
    generateStart(gen);
    
    brain = new Brain(eRadiation,gen,dist);
    // calculate color
}

Agent::Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain iBrain, float eRadiation){ // split spawn

    brain = new Brain(eRadiation,iBrain);
}

void Agent::generateStart(std::mt19937 gen){
// https://www.geeksforgeeks.org/cpp/how-to-generate-random-number-in-range-in-cpp/

}

void Agent::updateSpeed(float deltaEnergy){ // increase / decrease adrenalin
    float deltaSpeed = 1/(3*cSpeed)*deltaEnergy-0.2*speed;
    speed += deltaSpeed;
    blue = std::round(cSpeed*255);
    
    if(speed <0){
        speed = 0;
    }
}

void Agent::updateHealth(float deltaEnergy){
    if (cHealth <0.2){
        cHealth = 0.2;
    }
    else if(cHealth >1){
        cHealth = 1;
    }

    float deltaHealth = (1/10*cHealth)*health*deltaEnergy-0.5*health;
    if(deltaHealth < -2){
        deltaHealth = -2;
    }
    energy-=deltaEnergy;
    health+=deltaHealth;
    if (health < 0){
        health = 0;
    }
    green = std::round(cHealth*255);
    
}