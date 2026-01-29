#include <iostream>
#include "Environment/environment.h"


int main(){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    

    Agent a = Agent(0.0, gen);
    std::cout<<a.energy;
}