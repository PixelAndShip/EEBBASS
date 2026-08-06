#include <iostream>
#include "data_management/sim_manager.h"

#include "test.h"

int main()
{
    std::string fileName = "customBrain.txt";
    std::string simTypeChoiceStr = "";
    std::cout << "Enter simulation type: ";
    std::cin >> simTypeChoiceStr;
    if (simTypeChoiceStr == "v")
    {
        Environment env = Environment(3000);
        env.manageVisualizedSimulation();
        return 0;
    }
    else if (simTypeChoiceStr == "c")
    {
        Agent *customA = new Agent(fileName, true, 4000, 200, 200, 0.6, {200, 200, 255, 255}, 300, 300, 4);
        customA->getBrain().logBrain();
        return 0;
    }
    //  Agent(std::string fileName, bool iP, int iEID, float iH, float iE, float iPD, UnitColor iC, int ix, int iy, float iS);
    std::string eC = "";
    std::cout << "Enter simulation environment count: ";
    std::cin >> eC;
    try
    {

        int iEC = std::stoi(eC);
        std::cout << "Starting simulation with " << eC << " environments!";
        SimManager sm = SimManager(iEC);
        sm.runSimulation();
    }
    catch (...)
    {
        std::cout << "Custom environment count failed, starting with default 8 environments!";
        SimManager sm = SimManager();
        sm.runSimulation();
    }

    // Agent a = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    // Agent b = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    // Agent c = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    // Plant p = Plant();

    // a.setX(240);
    // b.setX(50);
    // c.setX(111);
    // e.spider->PastAgents.insert({a.getCoords(), &a});
    // e.spider->PastAgents.insert({b.getCoords(), &b});
    // e.spider->PastAgents.insert({c.getCoords(), &c});

    // e.spider->PastPlants.insert({"26_25", &p});
    // for (auto a : e.spider->PastAgents)
    // {
    //     std::cout << "|||" << a.first << "|||" << "\n";
    //     for (InputNode *i : a.second->getBrain().getInputNodes())
    //     {
    //         Test::printBrain(i);
    //     }
    // }
    // e.manageMoment();

    // e.makeWindow();

    // Test::getType(&p);
    // Test::getType(&a);
    // Test::proximityCheck(&a);
}
