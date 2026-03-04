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

    Environment env;
    env.Agents.insert({"12_3",&a});
    env.Agents.insert({"12_4",&a});
    env.Agents.insert({"11_2",&a});
    env.makeWindow();
    std::vector<std::string> cords = env.agentProximityCheck("12_3");
    for (std::string c : cords){
        std::cout<<c;
    }


}