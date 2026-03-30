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

Agent::Agent(float eRadiation, std::mt19937& gen, std::uniform_int_distribution<>& dist): brain(eRadiation,gen,dist){
    
    generateStart(gen);
    
   
    // calculate color
}

Agent::Agent(float iHealth, float iEnergy,float iSpeed, float iSight, Brain& iBrain, float eRadiation):brain(eRadiation,iBrain){ // split spawn

    
}

void Agent::generateStart(std::mt19937& gen){
// https://www.geeksforgeeks.org/cpp/how-to-generate-random-number-in-range-in-cpp/

}

