#include <iostream>
#include "environment/environment.h"

#include "test.h"

int main()
{
    Environment e = Environment();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 7);

    e.gen = gen;
    e.dist = dist;

    Agent a = Agent(0.5, e.gen, e.dist);
    a.setEnergy(1);
    a.setHealth(1);
    a.setAgentColor({1, 2, 3, 4});
    a.setX(12);
    a.setY(3);

    Agent b = Agent(0.5, e.gen, e.dist);
    b.setEnergy(2);
    b.setHealth(4);
    b.setAgentColor({12, 22, 32, 42});
    b.setX(11);
    b.setY(3);

    e.spider.Agents.insert({a.getCoords(), &a});
    e.spider.Agents.insert({b.getCoords(), &b});
    // for (InputNode *cn : a.getBrain().getInputNodes())
    // {
    //     Test::printBrain(cn);
    // }
    // e.makeWindow();
    // Plant p = Plant();
    // Test::getType(&p);
    // Test::getType(&a);
    // Test::proximityCheck(&a);
    e.manageMoment();
}
