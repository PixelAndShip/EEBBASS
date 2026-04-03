#include "spider.h"


void Spider::updateSpeed(float deltaEnergy, Agent* Self){ 
    Self->energy-=deltaEnergy;
    Self->speed += deltaEnergy;

    Self->blue = std::round(Self->speed*255);
    
    if(Self->speed <0){
        Self->speed = 0;
    }
    if (Self->energy < 0){
        Self->energy = 0;
    }
}


void Spider::updateHealth(float deltaEnergy, Agent* Self){
    
    Self->energy-=deltaEnergy;
    Self->health+=deltaEnergy;
    if (Self->health < 0){
        Self->health = 0;
    }
    if (Self->energy < 0){
        Self->energy = 0;
    }
    Self->green = std::round(Self->health*255);
    
}




void Spider::bite(Agent* Opponent, float energyCost){
    Opponent->health-=energyCost;
}

void Spider::move(Agent* Self, char Direction){
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
