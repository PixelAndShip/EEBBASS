#pragma once
class ConditionalInputNode;
class Agent;

class OutputNode{
public:
float weight;
float energyCost;
unsigned int key;

void updateSpeed(float deltaEnergy, Agent* Self);

void updateHealth(float deltaEnergy, Agent* Self);

void bite(Agent* Opponent, Agent* Self);

void move(Agent* Self, char Direction);

};