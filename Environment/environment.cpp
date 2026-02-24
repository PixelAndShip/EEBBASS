#include "environment.h"


void Environment::ManagePlantCount(){} // makes sure sim does not crash

void Environment::ManageAgentCount(){} // makes sure sim does not crash

void Environment::manageMoment(){} /*
loop through agents and their soroundings, each moment agent can perform several internal actions and 1 external action
this means each moment needs to be subdivided into submoments:
determening which agent gets to perform their external action first is determined by which agent has the higher speed stat

*/

void Environment::makeWindow(){
    InitWindow(800,800,"Environment");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();

}