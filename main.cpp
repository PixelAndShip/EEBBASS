#include <iostream>
#include "Environment/environment.h"
#include <random>
#include "test.h"

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,4);
    
    

    Agent a= Agent(0.9,gen,dist);

    
    
    a.energy = 1;
    a.health = 1.0;
    
    for (ConditionalInputNode* cn :a.brain.conditionalInputNodes){
        Test::printBrain(cn);
    }
    


}





