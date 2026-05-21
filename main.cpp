#include <iostream>
#include "environment/environment.h"

#include "test.h"

int main()
{
    Environment e = Environment();
    Agent a = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    Agent b = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    Agent c = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    Plant p = Plant();
    a.setX(24);
    b.setX(23);
    c.setX(111);
    e.spider->PastAgents.insert({a.getCoords(), &a});
    e.spider->PastAgents.insert({b.getCoords(), &b});
    e.spider->PastAgents.insert({c.getCoords(), &c});

    e.spider->PastPlants.insert({"26_25", &p});
    for (auto a : e.spider->PastAgents)
    {
        std::cout << "|||" << a.first << "|||" << "\n";
        for (InputNode *i : a.second->getBrain().getInputNodes())
        {
            Test::printBrain(i);
        }
    }
    e.manageMoment();

    // e.makeWindow();

    // Test::getType(&p);
    // Test::getType(&a);
    // Test::proximityCheck(&a);
}
