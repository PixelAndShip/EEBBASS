#include <iostream>
#include "Environment/environment.h"


int main(){
   
    srand (static_cast <unsigned> (time(0)));

    Agent a = Agent(0.0);
    std::cout<<a.energy;
}