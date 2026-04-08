#include <iostream>
#include "Environment/environment.h"

#include "test.h"

int main(){
    Environment e = Environment();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,7);

    e.gen = gen;
    e.dist = dist;
    
    

    Agent a= Agent(0.75,e.gen,e.dist);

    
    
    a.energy = 1;
    a.health = 1.0;
    
    for (InputNode* cn : a.brain.inputNodes){
        Test::printBrain(cn);
    }
    


}





