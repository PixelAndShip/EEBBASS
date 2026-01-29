#include <iostream>
#include "Environment/environment.h"


int main(){
    
   

    Agent a;
   
    a.energy = 1;
    a.health = 1.0;
    std::cout<<a.energy;
    a.updateHealth();
    std::cout<<"\n"<<a.health<<" "<< a.energy;
}