#include "environment.h"
#include <vector>

void Environment::managePlantCount(){} // makes sure sim does not crash

void Environment::manageAgentCount(){} // makes sure sim does not crash

void Environment::manageMoment(){
    /*
    loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
    this means each moment needs to be subdivided into submoments:
    determening which agent gets to perform their external action first is determined by which agent has the higher speed stat
    */
    std::unordered_map<std::string,bool> managedAgentCoordinates = {};
    for (std::string coords: spider.proximateAgents){
        
        if (managedAgentCoordinates.find(coords)!=managedAgentCoordinates.end()){
            continue;
        }
      
        manageSubMoment(coords,&managedAgentCoordinates);
    }

} 

void Environment::manageSubMoment(std::string coords,std::unordered_map<std::string,bool>* managedAgentCoordinates){
    // go through sorounding agent brains and determine first actions, add managed to managedAgentCoordinates
    spider.proximateAgents = agentProximityCheck(coords);
    
    
    for(std::string agentCoords : spider.proximateAgents){
        managedAgentCoordinates->insert({agentCoords,true});
    }
}

OutputNode Environment::getAction(InputNode* parentNode){
    
}


std::vector<std::string> Environment::agentProximityCheck(std::string coords){
    // check agent radius for other agents
    std::vector<std::string> agentCoordsInProximity = {};
    auto _pos = coords.find("_");
    int aX,aY;
    try{
        aX = std::stoi(coords.substr(0, _pos));
        aY = std::stoi(coords.substr(_pos + 1));
    }
    catch(...){
        return agentCoordsInProximity;
    }
    std::vector<std::string> InRadius = {"0_-1","1_-1","1_0","1_1","0_1","-1_1","-1_0","-1_-1"};
    for (std::string r : InRadius){
        auto _pos = r.find("_");
        int dx = std::stoi(r.substr(0, _pos));
        int dy = std::stoi(r.substr(_pos + 1));
        if(spider.Agents.find((std::to_string(aX+dx))+"_"+std::to_string(aY+dy))!=spider.Agents.end()){
            agentCoordsInProximity.push_back(std::to_string(aX+dx)+"_"+std::to_string(aY+dy));
        }
    }

    return agentCoordsInProximity;
}




void Environment::makeWindow(){
    InitWindow(800,800,"Environment");
    SetTargetFPS(60);

    
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        manageMoment(); // implement
        DrawCircle(100,200,10,RED);
       
        EndDrawing();
    }
    CloseWindow();

}