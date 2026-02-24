#include "environment.h"


void Environment::ManagePlantCount(){} // makes sure sim does not crash

void Environment::ManageAgentCount(){} // makes sure sim does not crash

void Environment::manageMoment(){
/*
loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
this means each moment needs to be subdivided into submoments:
determening which agent gets to perform their external action first is determined by which agent has the higher speed stat
*/
    std::unordered_map<std::string,bool> managedAgentCoordinates = {};
    for (const auto& data: Agents){
        std::string key = data.first;
        if (managedAgentCoordinates.find(key)!=managedAgentCoordinates.end()){
            continue;
        }
        Agent* agent = data.second;
        manageSubMoment(agent,&managedAgentCoordinates);
    }

} 

void manageSubMoment(Agent* agent,std::unordered_map<std::string,bool>* managedAgentCoordinates){
    // go through sorounding agent brains and determine first actions, add managed to managedAgentCoordinates
}

void Environment::makeWindow(){
    InitWindow(800,800,"Environment");
    SetTargetFPS(60);

    
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        manageMoment(); // implement
        
       
        EndDrawing();
    }
    CloseWindow();

}