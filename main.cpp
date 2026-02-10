#include <iostream>
#include "Environment/environment.h"
#include <random>

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,20);
    
    

    Agent a= Agent(0.2,gen,dist);
    
    a.energy = 1;
    a.health = 1.0;
    
}