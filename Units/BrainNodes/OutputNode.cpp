#include "OutputNode.h"
#include "Units/agent.h"


void OutputNode::updateSpeed(float deltaEnergy, Agent* Self){ // increase / decrease adrenalin
    float deltaSpeed = 1/(3*Self->cSpeed)*deltaEnergy-0.2*Self->speed;
    Self->speed += deltaSpeed;
    Self->blue = std::round(Self->cSpeed*255);
    
    if(Self->speed <0){
        Self->speed = 0;
    }
}


void OutputNode::updateHealth(float deltaEnergy, Agent* Self){
    if (Self->cHealth < 0.2){
        Self->cHealth = 0.2;
    }
    else if(Self->cHealth >1){
        Self->cHealth = 1;
    }

    float deltaHealth = (1/10*Self->cHealth)*Self->health*deltaEnergy-0.5*Self->health;
    if(deltaHealth < -2){
        deltaHealth = -2;
    }
    Self->energy-=deltaEnergy;
    Self->health+=deltaHealth;
    if (Self->health < 0){
        Self->health = 0;
    }
    Self->green = std::round(Self->cHealth*255);
    
}




void OutputNode::bite(Agent* Opponent, Agent* Self){
    Opponent->health-=energyCost;
}

void OutputNode::move(Agent* Self, char Direction){
    switch (Direction){
        case 'u':
            Self->y-=1;
        case 'd':
            Self->y+=1;
        case 'l':
            Self->x-=1;
        case 'r':
            Self->x+=1;

    }
}
